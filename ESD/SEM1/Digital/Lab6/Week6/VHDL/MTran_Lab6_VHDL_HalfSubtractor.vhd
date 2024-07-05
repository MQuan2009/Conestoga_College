library ieee;
use ieee.std_logic_1164.ALL;

entity MTran_Lab6_VHDL_HalfSubtractor is
    port
	 (
		A : in  std_logic_vector (3 downto 0);
      B : in  std_logic_vector (7 downto 4);
      Difference   : out  std_logic_vector (3 downto 0);
      Borrow       : out  std_logic_vector (3 downto 0)
	  );
end MTran_Lab6_VHDL_HalfSubtractor;

architecture Behavioral of MTran_Lab6_VHDL_HalfSubtractor is
begin
	Difference <= A xor B;
	Borrow <= (not A) and B;
end Behavioral;