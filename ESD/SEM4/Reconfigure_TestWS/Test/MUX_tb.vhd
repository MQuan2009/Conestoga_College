library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity Mux_tb is
end Mux_tb;

architecture test of Mux_tb is

    -- Component Declaration
    component Mux8to1
    	port
	(
	   D   : in  std_logic_vector(7 downto 0);
      	   Sel : in  std_logic_vector(2 downto 0);
           O   : out std_logic
    	);
    end component;

    -- Signals Declaration
    signal D_tb    : std_logic_vector(7 downto 0) := "00000000";
    signal Sel_tb  : std_logic_vector(2 downto 0) := "000";
    signal O_tb    : std_logic;

begin

    -- Instantiate the Decoder
    UUT: Mux8to1
        port map
	(
       	    D    => D_tb,
            Sel  => Sel_tb,
            O    => O_tb
        );

    -- Process
    process
    begin
        -- Initial Data (Test case)
        D_tb <= "10101010";

	-- Test Select
 	for i in 0 to 7 loop
            Sel_tb <= std_logic_vector(to_unsigned(i, 3));
            wait for 10 ns;
        end loop;
        -- Done
        wait;
    end process;

end test;