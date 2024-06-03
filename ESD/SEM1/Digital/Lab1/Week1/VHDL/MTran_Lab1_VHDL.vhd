library ieee;
use ieee.std_logic_1164.all;  --should be same through all the lab, except comparator

-- declaring input and output
entity MTran_Lab1_VHDL_AndGate is
    port 
	 -- a,b are SW1 and SW2, output is LEDR0
	 (
        a, b   : in  std_logic;
        output : out std_logic --end of declaration no more ';'
    );
end MTran_Lab1_VHDL_AndGate;

-- describing the relationship between output and input
architecture behavioral of MTran_Lab1_VHDL_AndGate is --'behavioral' is a block name (must not same name in library)
begin
    output <= a and b; -- <=: assigning varible 'output' 
end behavioral;
 