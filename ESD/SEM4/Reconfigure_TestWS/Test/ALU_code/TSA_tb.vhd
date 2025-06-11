library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity TSA_tb is
end TSA_tb;

architecture Behavioral of TSA_tb is
    -- Inputs to UUT
    signal EN     : STD_LOGIC := '0';
    signal ASEL   : STD_LOGIC_VECTOR(2 downto 0) := (others => '0');
    signal DATA   : STD_LOGIC_VECTOR(7 downto 0) := (others => '0');
    -- Flattened register file: reg7=0x01,6=0x23,5=0x45,4=0x67,3=0x89,2=0xAB,1=0xCD,0=0xEF
    signal REGS   : STD_LOGIC_VECTOR(63 downto 0) := x"0123456789ABCDEF";
    -- Output from UUT
    signal OUTPUT : STD_LOGIC_VECTOR(7 downto 0);

    -- Viewable unpacked registers
    type reg_array_t is array (0 to 7) of STD_LOGIC_VECTOR(7 downto 0);
    signal reg_view : reg_array_t;
begin
    -- Instantiate the combinational TSA
    UUT: entity work.TSA(Combinational)
       port map (
         EN     => EN,
         ASEL   => ASEL,
         DATA   => DATA,
         REGS   => REGS,
         OUTPUT => OUTPUT
       );

    -- Generate unpacked view of each byte in REGS
    register_view: for i in 0 to 7 generate
        reg_view(i) <= REGS((i*8 + 7) downto (i*8));
    end generate;

    -- Stimulus process
    stim_proc : process
    begin
        -- 1) EN = 0 ? OUTPUT = "00000000"
        EN   <= '0';
        ASEL <= "000";
        DATA <= x"AA";
        wait for 10 ns;
        
        -- 2) ASEL="000", EN=1 ? pass DATA
        EN   <= '1';
        ASEL <= "000";
        DATA <= x"55";
        wait for 10 ns;

        -- 3) ASEL="001" ? reg1 = CD
        ASEL <= "001";
        wait for 10 ns;

        -- 4) ASEL="010" ? reg2 = AB
        ASEL <= "010";
        wait for 10 ns;

        -- 5) ASEL="011" ? reg3 = 89
        ASEL <= "011";
        wait for 10 ns;

        -- 6) ASEL="100" ? reg4 = 67
        ASEL <= "100";
        wait for 10 ns;

        -- 7) ASEL="101" ? reg5 = 45
        ASEL <= "101";
        wait for 10 ns;

        -- 8) ASEL="110" ? reg6 = 23
        ASEL <= "110";
        wait for 10 ns;

        -- 9) ASEL="111" ? reg7 = 01
        ASEL <= "111";
        wait for 10 ns;

        -- Hold outputs and reg_view for waveform inspection
        wait;
    end process;

end Behavioral;

