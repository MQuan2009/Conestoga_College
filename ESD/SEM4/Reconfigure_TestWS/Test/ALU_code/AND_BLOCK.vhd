library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity AND_BLOCK is
    Port (
        CLK    : in  STD_LOGIC;
        EN     : in  STD_LOGIC;
        A      : in  STD_LOGIC_VECTOR(7 downto 0);
        B      : in  STD_LOGIC_VECTOR(7 downto 0);
        RESULT : out STD_LOGIC_VECTOR(7 downto 0)
    );
end AND_BLOCK;

architecture Behavioral of AND_BLOCK is
begin
    process(A, B, EN)
    begin
        if EN = '1' then
            RESULT <= A and B;
        end if;
    end process;
end Behavioral;

