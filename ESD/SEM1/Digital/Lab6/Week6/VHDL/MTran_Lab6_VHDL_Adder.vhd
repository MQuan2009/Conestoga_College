library ieee;
use ieee.std_logic_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity MTran_Lab6_VHDL_Adder is
    port
	 (
		mode  : in   std_logic_vector (9 downto 8);
		A     : in   std_logic_vector (3 downto 0);
      B     : in   std_logic_vector (7 downto 4);
      LED   : out  std_logic_vector (7 downto 0);
		HEX1  : out  std_logic_vector (6 downto 0);
		HEX0  : out  std_logic_vector (6 downto 0)
		
	  );
end MTran_Lab6_VHDL_Adder;

architecture Behavioral of MTran_Lab6_VHDL_Adder is

-------------------------- Component Declaration---------------------------------

--Half Adder
component MTran_Lab6_VHDL_HalfAdder is
    port
	 (
		A     : in   std_logic_vector (3 downto 0);
      B     : in   std_logic_vector (7 downto 4);
      Sum   : out  std_logic_vector (3 downto 0);
      Carry : out  std_logic_vector (3 downto 0)
	  );
end component;

--Half Subtractor
component MTran_Lab6_VHDL_HalfSubtractor is
    port
	 (
		A            : in   std_logic_vector (3 downto 0);
      B            : in   std_logic_vector (7 downto 4);
      Difference   : out  std_logic_vector (3 downto 0);
      Borrow       : out  std_logic_vector (3 downto 0)
	  );
end component;

--Full Adder
component MTran_Lab6_VHDL_FullAdder is
    port
	 (
		A            : in   std_logic_vector (3 downto 0);
      B            : in   std_logic_vector (3 downto 0);
		Cin			 : in   std_logic;
      Sum          : out  std_logic_vector (3 downto 0);
      CarryOut     : out  std_logic
	  );
end component;

-------------------------- End of Component Declaration--------------------------

-------------------------- Function Declaration----------------------------------

-- Function to calculate Sum of Half Adder

-- Seven Segment Display
function SevenSegmentDisplay(Number : integer) return std_logic_vector is
begin
	case (Number) is
	when 0 => return "1000000";
	when 1 => return "1111001";
	when 2 => return "0100100";
	when 3 => return "0110000";
	when 4 => return "0011001";
	when 5 => return "0010010";
	when 6 => return "0000010";
	when 7 => return "1111000";
	when 8 => return "0000000";
	when 9 => return "0010000";
	when others => return "1111111";
	end case;
end SevenSegmentDisplay;

--------------------------End of Function Declaration--------------------------

--------------------------Variable and Signal Declaration----------------------

signal Sum_HA : std_logic_vector(3 downto 0);
signal Carry_HA : std_logic_vector(3 downto 0);

signal Difference_HS: std_logic_vector(3 downto 0);
signal Borrow_HS: std_logic_vector(3 downto 0);

signal Sum_FA : std_logic_vector(3 downto 0);
signal Carry_FA : std_logic;
signal Cin_FA: std_logic;

signal TotalSum : integer;
signal tenth    : integer;
signal unit     : integer;
--------------------------End of Variable and Signal Declaration---------------

begin
-- Main code

-- Component
HalfAdder: MTran_Lab6_VHDL_HalfAdder
	port map(
		A => A,
		B => B,
		Sum => Sum_HA,
		Carry => Carry_HA
	);
	
HalfSubtractor: MTran_Lab6_VHDL_HalfSubtractor
	port map(
		A => A,
		B => B,
		Difference => Difference_HS,
		Borrow => Borrow_HS
	);
	
FullAdder: MTran_Lab6_VHDL_FullAdder
	port map(
		A => A,
		B => B,
		Cin => Cin_FA,
		Sum => Sum_FA,
		CarryOut => Carry_FA
	);
	

process(mode, A, B)
begin
	-- Reset Seven Segment Display and LED
	HEX1 <= "1111111";
	HEX0 <= "1111111";
	LED  <= "00000000";
	
	-- Change mode
	if mode = "00" then 				-- Half Adder mode 
		LED(3 downto 0) <= Sum_HA;
		LED(7 downto 4) <= Carry_HA;
	elsif mode = "01" then        -- Half Subtractor mode
		LED(3 downto 0) <= Difference_HS;
		LED(7 downto 4) <= Borrow_HS;
	elsif mode = "10" then        -- Full Adder with Cin = 0  mode
		Cin_FA <= '0';
		LED(3 downto 0) <= Sum_FA;
		LED(4) <= Carry_FA;
		
		TotalSum <= to_integer(unsigned(Carry_FA & Sum_FA));
		tenth <= TotalSum / 10;
		unit  <= TotalSum mod 10;
		
		HEX1 <= SevenSegmentDisplay(tenth);
		HEX0 <= SevenSegmentDisplay(unit);
	elsif mode = "11" then        -- Full Adder with Cin = 1  mode
		Cin_FA <= '1';
		LED(3 downto 0) <= Sum_FA;
		LED(4) <= Carry_FA;
		
		TotalSum <= to_integer(unsigned(Carry_FA & Sum_FA));
		tenth <= TotalSum / 10;
		unit  <= TotalSum mod 10;
		
		HEX1 <= SevenSegmentDisplay(tenth);
		HEX0 <= SevenSegmentDisplay(unit);
	end if;
end process;
end Behavioral;