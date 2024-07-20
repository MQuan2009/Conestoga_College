library ieee;
use ieee.std_logic_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity MTran_Lab7_VHDL_MagnitudeComparator is
port
(
	-- Inputs
	CLOCK_50 : in  std_logic;
	A        : in  std_logic_vector (3 downto 0);
	B        : in  std_logic_vector (3 downto 0);
	mode     : in  std_logic;
	 
	-- Outputs
	LED   : out std_logic_vector (2 downto 0);
	HEX0  : out std_logic_vector (6 downto 0);
	HEX1  : out std_logic_vector (6 downto 0);
	HEX2  : out std_logic_vector (6 downto 0);
	HEX3  : out std_logic_vector (6 downto 0);
	HEX4  : out std_logic_vector (6 downto 0);
	HEX5  : out std_logic_vector (6 downto 0)
	
);
end entity;

architecture Behavioral of MTran_Lab7_VHDL_MagnitudeComparator is

-- Module declaration
component MTran_Lab7_VHDL_Timer is 
port 
(
	-- Inputs
	Clk     : in std_logic;
	Enable  : in std_logic;
	
	-- Outputs
	Seconds : inout integer;
	Minutes : inout integer;
	Hours   : inout integer
);
end component;

-- Singal declaration
signal intA        : integer := 0;
signal intB        : integer := 0;

signal Enable      : std_logic;

signal Seconds     : integer := 0;
signal Minutes     : integer := 0;
signal Hours       : integer := 0;


-- Function
function SevenSegmentDisplay(Number : integer) return std_logic_vector is
begin
	case (Number) is
	when 0	   => return "1000000";
	when 1		=> return "1111001";
	when 2		=> return "0100100";
	when 3 		=> return "0110000";
	when 4		=> return "0011001";
	when 5 		=> return "0010010";
	when 6		=> return "0000010";
	when 7      => return "1111000";
	when 8      => return "0000000";
	when 9      => return "0010000";
	when others => return "1111111";
	end case;
end SevenSegmentDisplay;


-- Main code
begin
	-- Module instatiate
	module_timer: MTran_Lab7_VHDL_Timer
	port map
	(
		Clk 	=> CLOCK_50,
		Enable  => Enable,
		Seconds => Seconds,
		Minutes => Minutes,
		Hours   => Hours
	);
	
	-- Process
	process(CLOCK_50, Seconds, Minutes, Hours, A, B) is 
	begin
		if mode = '0' then
			-- Disable Timer
			Enable <= '0';
			
			-- Reset HEX2
			HEX2 <= "1111111";
			
			-- Convert A and B from std_logic to integer
			intA <= to_integer(unsigned(A));
			intB <= to_integer(unsigned(B));
			
			-- Display A and B in decimal
			
			-- A
			HEX0 <= SevenSegmentDisplay(intA mod 10);
			HEX1 <= SevenSegmentDisplay(intA / 10);
			
			-- B
			HEX4 <= SevenSegmentDisplay(intB mod 10);
			HEX5 <= SevenSegmentDisplay(intB / 10);			
			
			-- Compare A and B
			if intA > intB then 
				LED <= "100";
				HEX3 <= "0100111";
			elsif intA = intB then 
				LED <= "010";
				HEX3 <= "0110111";
			else
				LED <= "001";
				HEX3 <= "0110011";
			end if;
			
		else
			Enable <= '1';
			LED <= "000";
			-- Display Second
			HEX0 <= SevenSegmentDisplay(Seconds mod 10);
			HEX1 <= SevenSegmentDisplay(Seconds / 10);
			
			-- Display Minute
			HEX2 <= SevenSegmentDisplay(Minutes mod 10);
			HEX3 <= SevenSegmentDisplay(Minutes / 10);
			
			-- Display Hour
			HEX4 <= SevenSegmentDisplay(Hours mod 10);
			HEX5 <= SevenSegmentDisplay(Hours / 10);
			

		end if;
		
	end process;	

end architecture;