library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity TSA is
    Port (
        EN     : in  STD_LOGIC;
        ASEL   : in  STD_LOGIC_VECTOR(2 downto 0);    -- 0 = DATA, 1?7 = reg 1?7
        DATA   : in  STD_LOGIC_VECTOR(7 downto 0);    -- external input
        REGS   : in  STD_LOGIC_VECTOR(63 downto 0);   -- flattened register file
        OUTPUT : out STD_LOGIC_VECTOR(7 downto 0)
    );
end TSA;

architecture Combinational of TSA is
begin
    process(EN, ASEL, DATA, REGS)
        variable idx : integer range 0 to 7;
    begin
        if EN = '1' then
            if ASEL = "000" then
                OUTPUT <= DATA;
            else
                idx    := to_integer(unsigned(ASEL));
                OUTPUT <= REGS(idx*8 + 7 downto idx*8);
            end if;
        else
            OUTPUT <= (others => '0');
        end if;
    end process;
end Combinational;

