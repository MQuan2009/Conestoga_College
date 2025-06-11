library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity OctalDecoder_tb is
end OctalDecoder_tb;

architecture test of OctalDecoder_tb is

    -- Component Declaration
    component MTran_Lab5_VHDL_OctalDecoder
        port
	(
            EN : in  std_logic;
            A  : in  std_logic_vector(2 downto 0);
            O  : out std_logic_vector(7 downto 0)
        );
    end component;

    -- Signals Declaration
    signal EN_tb : std_logic := '0';
    signal A_tb  : std_logic_vector(2 downto 0) := "000";
    signal O_tb  : std_logic_vector(7 downto 0);

begin

    -- Instantiate the Decoder
    UUT: MTran_Lab5_VHDL_OctalDecoder
        port map
	(
            EN => EN_tb,
            A  => A_tb,
            O  => O_tb
        );

    -- Process
    process
    begin
        -- Initially disabled (Test case 1)
        EN_tb <= '0';
 	for i in 0 to 7 loop
            A_tb <= std_logic_vector(to_unsigned(i, 3));
            wait for 10 ns;
        end loop;

        -- Enable and cycle through all inputs (Test case 2)
        EN_tb <= '1';
        for i in 0 to 7 loop
            A_tb <= std_logic_vector(to_unsigned(i, 3));
            wait for 10 ns;
        end loop;

        -- Disable again
        EN_tb <= '0';
        wait for 10 ns;

        -- Done
        wait;
    end process;

end test;
