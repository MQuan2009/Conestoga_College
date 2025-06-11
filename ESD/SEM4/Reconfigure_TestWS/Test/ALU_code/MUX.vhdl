-- REG_MUX.vhd
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity REG_MUX is
    Port (
        REGS   : in  STD_LOGIC_VECTOR(63 downto 0); -- Flattened register file
        SEL_A  : in  STD_LOGIC_VECTOR(2 downto 0);
        SEL_B  : in  STD_LOGIC_VECTOR(2 downto 0);
        Abus   : out STD_LOGIC_VECTOR(7 downto 0);
        Bbus   : out STD_LOGIC_VECTOR(7 downto 0)
    );
end REG_MUX;

architecture Behavioral of REG_MUX is
    type reg_array is array(0 to 7) of STD_LOGIC_VECTOR(7 downto 0);
    signal registers : reg_array;
begin
    -- Unpack flat register vector into array
    process(REGS)
    begin
        for i in 0 to 7 loop
            registers(i) <= REGS(i*8 + 7 downto i*8);
        end loop;
    end process;

    Abus <= registers(to_integer(unsigned(SEL_A)));
    Bbus <= registers(to_integer(unsigned(SEL_B)));
end Behavioral;

