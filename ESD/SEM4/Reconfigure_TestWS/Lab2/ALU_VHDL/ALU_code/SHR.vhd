library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity SHR is
    Port (
        CLK       : in  STD_LOGIC;
        EN        : in  STD_LOGIC;
        A         : in  STD_LOGIC_VECTOR(7 downto 0);
        SEL       : in  STD_LOGIC_VECTOR(2 downto 0);    -- Select register index (3 bits for 8 registers)
        RESULT    : out STD_LOGIC_VECTOR(7 downto 0);
        CARRYOUT  : out STD_LOGIC
    );
end SHR;

architecture Behavioral of SHR is
begin
    process(A, EN, SEL)
    begin
        if EN = '1' then
            CARRYOUT <= A(0);
            RESULT   <= '0' & A(7 downto 1);
        end if;
    end process;
end Behavioral;

