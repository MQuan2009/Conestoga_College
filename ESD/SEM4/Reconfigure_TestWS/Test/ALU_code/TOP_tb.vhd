library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity TOP_TB is
end TOP_TB;

architecture sim of TOP_TB is

    -- Signals
    signal CLK  	 : STD_LOGIC := '0';
    signal OP 	         : STD_LOGIC_VECTOR(3 downto 0);
    signal EN    	 : STD_LOGIC;
    signal SEL    	 : STD_LOGIC_VECTOR(2 downto 0);
    signal SEL_A    	 : STD_LOGIC_VECTOR(2 downto 0);
    signal SEL_B    	 : STD_LOGIC_VECTOR(2 downto 0);
    signal DATA_IN   	 : STD_LOGIC_VECTOR(7 downto 0);
    signal CARRYIN  	 : STD_LOGIC := '0';
    signal RESULT    	 : STD_LOGIC_VECTOR(7 downto 0);
    signal C         	 : STD_LOGIC;
    signal Z             : STD_LOGIC;
    signal S		 : STD_LOGIC;
    signal V		 : STD_LOGIC;
    signal REGISTER_FILE : STD_LOGIC_VECTOR(63 downto 0);

    type reg_array is array (0 to 7) of STD_LOGIC_VECTOR(7 downto 0);
    signal reg_view : reg_array;

    constant CLK_PERIOD : time := 10 ns;

begin

    -- Clock generation
    clk_process : process
    begin
        while true loop
            CLK <= '0';
            wait for CLK_PERIOD/2;
            CLK <= '1';
            wait for CLK_PERIOD/2;
        end loop;
    end process;

    -- DUT
    DUT: entity work.TOP
        port map (
            CLK      	  => CLK,
            OP       	  => OP,
            EN       	  => EN,
            SEL      	  => SEL,
            SEL_A    	  => SEL_A,
            SEL_B    	  => SEL_B,
            DATA_IN  	  => DATA_IN,
            CARRYIN 	  => CARRYIN,
            RESULT   	  => RESULT,
            C	 	  => C,
            V	 	  => V,
            Z	 	  => Z,
            S	 	  => S,	    
	    REGISTER_FILE => REGISTER_FILE
        );
    -- Reconstruct the array from the flat vector
    process(REGISTER_FILE)
    begin
    	for i in 0 to 7 loop
            reg_view(i) <= REGISTER_FILE(i*8 + 7 downto i*8);
    	end loop;
    end process;

    -- Stimulus process
    stim_proc: process
    begin

        -- Step 1: TSA to load 2 values into reg2 and reg6
        wait for CLK_PERIOD;
        EN      <= '1';
        OP      <= "0000";             -- TSA
        DATA_IN <= "11001000";         -- -56 (signed 8 bits)
        SEL     <= "010";              -- reg2
	SEL_A   <= "000";              -- Transfer Data In
        wait for CLK_PERIOD;

        DATA_IN <= "01100100";         -- 5
        SEL     <= "110";              -- reg6
        wait for CLK_PERIOD;

        -- Step 2: INC reg2 twice
        OP      <= "0001";             -- INC
        SEL     <= "010";              -- write back to reg2
        SEL_A   <= "010";              -- use reg2 as A
        wait for CLK_PERIOD;

        wait for CLK_PERIOD;

        -- Step 3: DEC reg2 twice
        OP <= "0010";                  -- DEC
        wait for CLK_PERIOD;
        wait for CLK_PERIOD;

        -- Step 4: ADD reg2 + reg6 -> store to reg0
        OP      <= "0011";             -- ADD
        SEL     <= "000";              -- result to reg0
        SEL_A   <= "010";              -- A = reg2
        SEL_B   <= "110";              -- B = reg6
        CARRYIN <= '0';
        wait for CLK_PERIOD;

        -- Step 5: SUB reg2 - reg6 - 1 => store to reg1
        EN      <= '1';
        OP      <= "0100";           -- SUB
        SEL     <= "001";            -- reg1
        SEL_A   <= "010";            -- reg2
        SEL_B   <= "110";            -- reg6
        CARRYIN <= '1';
        wait for CLK_PERIOD;

        -- Step 6: AND reg2 & reg6 -> reg3
        OP      <= "0101";           -- AND
        SEL     <= "011";            -- reg3
        wait for CLK_PERIOD;

        -- Step 7: OR reg2 | reg6 -> reg4
        OP      <= "0110";           -- OR
        SEL     <= "100";            -- reg4
        wait for CLK_PERIOD;

        -- Step 8: XOR reg2 xor reg6 -> reg5
        OP      <= "0111";           -- XOR
        SEL     <= "101";            -- reg5
        wait for CLK_PERIOD;

        -- Step 9: NOT reg2 not reg6 -> reg7
        OP      <= "1000";           -- NOT
        SEL     <= "111";            -- reg7
        wait for CLK_PERIOD;

        -- Step 10: SHL reg2 -> reg2
        OP      <= "1001";           -- SHL
        SEL     <= "010";            -- write back to reg2
        SEL_A   <= "010";            -- use reg2 as A
        wait for CLK_PERIOD;

	wait for CLK_PERIOD;

        -- Step 11: SHR reg2 -> reg2
        OP      <= "1010";           -- SHR
        SEL     <= "010";            -- write back to reg2
        SEL_A   <= "010";            -- use reg2 as A
        wait for CLK_PERIOD;

	wait for CLK_PERIOD;


        -- Done
        EN <= '0';
        wait;

    end process;

end sim;

