library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity INC is
    Port (
        CLK     : in  STD_LOGIC;                       -- Clock
        EN      : in  STD_LOGIC;                       -- Enable
        SEL     : in  STD_LOGIC_VECTOR(2 downto 0);    -- Select register index (3 bits for 8 registers)
        A       : in  STD_LOGIC_VECTOR(7 downto 0);    -- Register value to increment
        RESULT  : out STD_LOGIC_VECTOR(7 downto 0);    -- Output register value (selected)
        CARRY   : out STD_LOGIC                        -- Carry from last increment stored
    );
end INC;

architecture Behavioral of INC is
    
begin

    process(A, EN, SEL)
        variable index : integer;
	variable temp  : STD_LOGIC_VECTOR(8 downto 0);
    begin
        index := to_integer(unsigned(SEL));
        if EN = '1' then
             -- Increment input A
             temp := std_logic_vector(unsigned('0' & A) + to_unsigned(1, 9));
             -- Store incremented value and carry in selected register
 	     RESULT <= temp(7 downto 0);
   	     CARRY  <= temp(8);
        end if;
    end process;
end Behavioral;
