library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity TSA is
    Port (
        CLK      : in  STD_LOGIC;
        EN       : in  STD_LOGIC;
        DATA     : in  STD_LOGIC_VECTOR(7 downto 0);
        OUTPUT   : out STD_LOGIC_VECTOR(7 downto 0)
    );
end TSA;

architecture Behavioral of TSA is
begin
    -- Load data into the selected register on rising edge of clk
    process(EN, DATA)
    begin
        if EN = '1' then
            OUTPUT <= DATA;
        end if;
    end process;
end Behavioral;

