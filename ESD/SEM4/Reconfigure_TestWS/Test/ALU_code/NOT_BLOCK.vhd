library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity NOT_BLOCK is
    Port (
        CLK    : in  STD_LOGIC;
        EN     : in  STD_LOGIC;
        A      : in  STD_LOGIC_VECTOR(7 downto 0);
        RESULT : out STD_LOGIC_VECTOR(7 downto 0)
    );
end NOT_BLOCK;

architecture Behavioral of NOT_BLOCK is
begin
    process(A, EN)
    begin
        if EN = '1' then
            RESULT <= not A;
        end if;
    end process;
end Behavioral;

