library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity TOP is
    Port (
        CLK       : in  STD_LOGIC;
        OP        : in  STD_LOGIC_VECTOR(3 downto 0);
        EN        : in  STD_LOGIC;
        SEL       : in  STD_LOGIC_VECTOR(2 downto 0);   -- Register destination
        SEL_A     : in  STD_LOGIC_VECTOR(2 downto 0);   -- A source register
        SEL_B     : in  STD_LOGIC_VECTOR(2 downto 0);   -- B source register
        DATA_IN   : in  STD_LOGIC_VECTOR(7 downto 0);   -- TSA input
        CARRYIN   : in  STD_LOGIC;                      -- For ADD
        RESULT    : out STD_LOGIC_VECTOR(7 downto 0);   -- Output of selected register
        C         : out STD_LOGIC;
        Z         : out STD_LOGIC;
        S	  : out STD_LOGIC;
        V	  : out STD_LOGIC;
	REGISTER_FILE : out STD_LOGIC_VECTOR(63 downto 0)
    );
end TOP;

architecture Behavioral of TOP is

    -- Register file: 8 registers of 8 bits
    type reg_array is array (0 to 7) of STD_LOGIC_VECTOR(7 downto 0);
    signal registers : reg_array := (others => (others => '0'));

    -- Flattened version of reg_file for MUX
    signal flat_regs : STD_LOGIC_VECTOR(63 downto 0);
    signal result_sel : STD_LOGIC_VECTOR(7 downto 0);

    -- Internal buses
    signal Abus, Bbus : STD_LOGIC_VECTOR(7 downto 0);

    -- Internal outputs from blocks
    signal TSA_out, INC_out, DEC_out, ADD_out           : STD_LOGIC_VECTOR(7 downto 0);
    signal SUB_out, AND_out, OR_out, XOR_out            : STD_LOGIC_VECTOR(7 downto 0);
    signal NOT_out, SHL_out, SHR_out                    : STD_LOGIC_VECTOR(7 downto 0);
    signal INC_carry, DEC_borrow, ADD_carry, SUB_borrow : STD_LOGIC;
    signal SHL_carry, SHR_carry : STD_LOGIC;

    -- Enable signals
    signal TSA_en, INC_en, DEC_en, ADD_en : STD_LOGIC;
    signal SUB_en, AND_en, OR_en, XOR_en : STD_LOGIC;
    signal NOT_en, SHL_en, SHR_en : STD_LOGIC;

begin

    -- Flatten the reg_file array
    process(registers)
    begin
        for i in 0 to 7 loop
            flat_regs(i*8 + 7 downto i*8) <= registers(i);
        end loop;
    end process;

    -- Store to REGISTER_FILE
    REGISTER_FILE <= flat_regs;

    -- Enable only the selected operation
    TSA_en <= EN when OP = "0000" else '0';
    INC_en <= EN when OP = "0001" else '0';
    DEC_en <= EN when OP = "0010" else '0';
    ADD_en <= EN when OP = "0011" else '0';
    SUB_en <= EN when OP = "0100" else '0';
    AND_en <= EN when OP = "0101" else '0';
    OR_en  <= EN when OP = "0110" else '0';
    XOR_en <= EN when OP = "0111" else '0';
    NOT_en <= EN when OP = "1000" else '0';
    SHL_en <= EN when OP = "1001" else '0';
    SHR_en <= EN when OP = "1010" else '0';

    -- Register MUX
    MUX_INST : entity work.REG_MUX
        port map (
            REGS   => flat_regs,
            SEL_A  => SEL_A,
            SEL_B  => SEL_B,
            Abus   => Abus,
            Bbus   => Bbus
        );

    -- TSA block
    TSA_INST : entity work.TSA
        port map (
            CLK    => CLK,
            EN     => TSA_en,
            DATA   => DATA_IN,
            OUTPUT => TSA_out
        );

    -- INC block
    INC_INST : entity work.INC
        port map (
            CLK    => CLK,
            EN     => INC_en,
            SEL    => SEL_A,  -- Still needed but unused in INC implementation
            A      => Abus,
            RESULT => INC_out,
            CARRY  => INC_carry
        );

    -- DEC block
    DEC_INST : entity work.DEC
        port map (
            CLK    => CLK,
            EN     => DEC_en,
            SEL    => SEL_A,  -- Still needed but unused in DEC implementation
            A      => Abus,
            RESULT => DEC_out,
            BORROW => DEC_borrow
        );

    -- ADD block
    ADD_INST : entity work.ADD
        port map (
            CLK      => CLK,
            EN       => ADD_en,
            CARRYIN  => CARRYIN,
            A        => Abus,
            B        => Bbus,
            RESULT   => ADD_out,
            CARRYOUT => ADD_carry
        );

    -- SUB block
    SUB_INST : entity work.SUB
        port map (
            CLK      => CLK,
            EN       => SUB_en,
            CARRYIN  => CARRYIN,
            A        => Abus,
            B        => Bbus,
            RESULT   => SUB_out,
            BORROW   => SUB_borrow
        );

    -- AND block
    AND_INST : entity work.AND_BLOCK
        port map (
            CLK      => CLK,
            EN       => ADD_en,
            A        => Abus,
            B        => Bbus,
	    RESULT   => AND_out
        );

    -- OR block
    OR_INST : entity work.OR_BLOCK
        port map (
            CLK      => CLK,
            EN       => OR_en,
            A        => Abus,
            B        => Bbus,
	    RESULT   => OR_out
        );

    -- XOR block
    XOR_INST : entity work.XOR_BLOCK
        port map (
            CLK      => CLK,
            EN       => XOR_en,
            A        => Abus,
            B        => Bbus,
	    RESULT   => XOR_out
        );

    -- NOT block
    NOT_INST : entity work.NOT_BLOCK
        port map (
            CLK      => CLK,
            EN       => NOT_en,
            A        => Abus,
	    RESULT   => NOT_out
        );

    -- SHL block
    SHL_INST : entity work.SHL
        port map (
            CLK      => CLK,
            EN       => SHL_en,
	    SEL      => SEL_A,  -- Still needed but unused in INC implementation
            A        => Abus,
	    RESULT   => SHL_out,
	    CARRYOUT => SHL_carry
        );

    -- SHR block
    SHR_INST : entity work.SHR
        port map (
            CLK      => CLK,
            EN       => SHR_en,
            SEL      => SEL_A,  -- Still needed but unused in INC implementation
            A        => Abus,
	    RESULT   => SHR_out,
	    CARRYOUT => SHR_carry
        );
    -- Register File Write Logic
    process(CLK)
        variable index : integer;
    begin
        if rising_edge(CLK) then
            if EN = '1' then
                index := to_integer(unsigned(SEL));
                case OP is
                    when "0000" => registers(index) <= TSA_out;
                    when "0001" => registers(index) <= INC_out;
                    when "0010" => registers(index) <= DEC_out;
                    when "0011" => registers(index) <= ADD_out;
                    when "0100" => registers(index) <= SUB_out;
                    when "0101" => registers(index) <= AND_out;
                    when "0110" => registers(index) <= OR_out;
                    when "0111" => registers(index) <= XOR_out;
                    when "1000" => registers(index) <= NOT_out;
                    when "1001" => registers(index) <= SHL_out;
                    when "1010" => registers(index) <= SHR_out;
                    when others => null;
                end case;
            end if;
        end if;
    end process;

    -- Output the current value of selected register
    RESULT <= registers(to_integer(unsigned(SEL)));

    -- Flags
    -- C (Carry flag)
    with OP select
        C <= INC_carry  when "0001",
             DEC_borrow when "0010",
             ADD_carry  when "0011",
             SUB_borrow when "0100",
             SHL_carry  when "1001",
             SHR_carry  when "1010",
             '0'        when others;

    -- Z (Zero flag)
    result_sel <= registers(to_integer(unsigned(SEL)));
    Z <= '1' when result_sel = x"00" else '0';
    
    -- S (Signed flag)
    S <= result_sel(7);
    -- V (Overflow flag)
    with OP select
        V <= INC_carry  when "0001",
             DEC_borrow when "0010",
             ADD_carry  when "0011",
             SUB_borrow when "0100",
             SHL_carry  when "1001",
             SHR_carry  when "1010",
             '0'        when others;

end Behavioral;

