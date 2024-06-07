library ieee;
use ieee.std_logic_1164.ALL;

entity MTran_Lab61_VHDL_HalfAdder is
    port
	 (
		mode: in std_logic;
		A : in  std_logic_vector (3 downto 0);
      B : in  std_logic_vector (7 downto 4);
      Sum   : out  std_logic_vector (3 downto 0);
      Carry : out  std_logic_vector (7 downto 4);
		HEX1  : out  std_logic_vector (6 downto 0);
		HEX0  : out  std_logic_vector (6 downto 0)
		
	  );
end MTran_Lab61_VHDL_HalfAdder;

architecture Behavioral of MTran_Lab61_VHDL_HalfAdder is
-- Function Declaration

-- Function to calculate Sum of Half Adder
function Sum_HalfAdder ( a : std_logic_vector (3 downto 0); b : std_logic_vector (7 downto 4)) return std_logic_vector is
begin
	return a xor b;
end Sum_HalfAdder;

-- Function to calculate Carry of Half Adder
function Carry_HalfAdder ( a : std_logic_vector (3 downto 0); b : std_logic_vector (7 downto 4)) return std_logic_vector is
begin
	return a and b;
end Carry_HalfAdder;

-- Function to calculate Difference of Half Subtractor
function Difference_HalfSubtractor ( a : std_logic_vector (3 downto 0); b : std_logic_vector (7 downto 4)) return std_logic_vector is
begin
	return a xor b;
end Difference_HalfSubtractor;

-- Function to calculate Borrow of Half Subtractor
function Borrow_HalfSubtractor ( a : std_logic_vector (3 downto 0); b : std_logic_vector (7 downto 4)) return std_logic_vector is
begin
	return (not a) and b;
end Borrow_HalfSubtractor;

-- Seven Segment Display
function SevenSegmentDisplay(Switch : std_logic_vector (3 downto 0)) return std_logic_vector is
begin
	case (Switch) is
	when "0000" => return "1000000";
	when "0001" => return "1111001";
	when "0010" => return "0100100";
	when "0011" => return "0110000";
	when "0100" => return "0011001";
	when "0101" => return "0010010";
	when "0110" => return "0000010";
	when "0111" => return "1111000";
	when "1000" => return "0000000";
	when "1001" => return "0010000";
	when "1010" => return "0001000";
	when "1011" => return "0000011";
	when "1100" => return "1000110";
	when "1101" => return "0100001";
	when "1110" => return "0000110";
	when "1111" => return "0001110";
	when others => return "1111111";
	end case;
end SevenSegmentDisplay;

-- End Function Declaration

begin
-- Main code
process(mode, A, B)
begin
	if mode = '0' then 
		Sum <= Sum_HalfAdder(A,B);
		HEX0 <= SevenSegmentDisplay(Sum_HalfAdder(A,B));
		Carry <= Carry_HalfAdder(A,B);
		HEX1 <= SevenSegmentDisplay(Carry_HalfAdder(A,B));
	elsif mode = '1' then 
		Sum <= Difference_HalfSubtractor(A,B);
		HEX0 <= SevenSegmentDisplay(Difference_HalfSubtractor(A,B));
		Carry <= Borrow_HalfSubtractor(A,B);
		HEX1 <= SevenSegmentDisplay(Borrow_HalfSubtractor(A,B));
	end if;
end process;
end Behavioral;