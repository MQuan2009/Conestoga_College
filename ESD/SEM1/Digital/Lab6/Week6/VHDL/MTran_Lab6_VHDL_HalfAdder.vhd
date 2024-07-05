library ieee;
use ieee.std_logic_1164.ALL;

entity MTran_Lab6_VHDL_HalfAdder is
    port
	 (
		A : in  std_logic_vector (3 downto 0);
      B : in  std_logic_vector (7 downto 4);
      Sum   : out  std_logic_vector (3 downto 0);
      Carry : out  std_logic_vector (3 downto 0)
	  );
end MTran_Lab6_VHDL_HalfAdder;

architecture Behavioral of MTran_Lab6_VHDL_HalfAdder is
begin
	Sum <= A xor B;
	Carry <= A and B;
end Behavioral;