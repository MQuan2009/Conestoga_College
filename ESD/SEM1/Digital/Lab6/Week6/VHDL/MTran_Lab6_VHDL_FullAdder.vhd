library ieee;
use ieee.std_logic_1164.ALL;

entity MTran_Lab6_VHDL_FullAdder is
    port
	 (
		A        : in   std_logic_vector (3 downto 0);
      B        : in   std_logic_vector (3 downto 0);
		Cin      : in   std_logic;
      Sum      : out  std_logic_vector (3 downto 0);
      CarryOut : out  std_logic
	  );
end MTran_Lab6_VHDL_FullAdder;

architecture Behavioral of MTran_Lab6_VHDL_FullAdder is
signal Carry: std_logic_vector (3 downto 0);
begin
	Sum(0) <= A(0) xor B(0) xor Cin;
	Carry(0) <= ( A(0) and B(0) ) or ( B(0) and Cin ) or ( A(0) and Cin );
	
	Sum(1) <= A(1) xor B(1) xor Carry(0);
	Carry(1) <= ( A(1) and B(1) ) or ( B(1) and Carry(0) ) or ( A(1) and Carry(0) );

	Sum(2) <= A(2) xor B(2) xor Carry(1);
	Carry(2) <= ( A(2) and B(2) ) or ( B(2) and Carry(1) ) or ( A(2) and Carry(1) );

	Sum(3) <= A(3) xor B(3) xor Carry(2);
	Carry(3) <= ( A(3) and B(3) ) or ( B(3) and Carry(2) ) or ( A(3) and Carry(2) );
	
	CarryOut <= Carry(3);

end Behavioral;