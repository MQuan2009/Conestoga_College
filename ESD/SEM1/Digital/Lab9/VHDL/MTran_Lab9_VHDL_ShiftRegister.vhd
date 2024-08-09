library ieee;
use ieee.std_logic_1164.all;

entity MTran_Lab9_VHDL_ShiftRegister is
port
(
	-- Inputs
	BTN   : in std_logic;
	Speed : in std_logic;
	Reset : in std_logic;
	SW    : in std_logic_vector (7 downto 0);
	
	-- Outputs
	LED   : out std_logic_vector (3 downto 0);
	HEX0  : out std_logic_vector (6 downto 0)
	
);
end entity;

architecture behavioral of MTran_Lab9_VHDL_ShiftRegister is

-- Signal Declaration
signal Counter : integer                       := 1;

signal Output  : std_logic_vector (3 downto 0) := "0000";
signal Sel     : std_logic_Vector (1 downto 0);

-- Function declaration
function SevenSegmentDisplay (Number : integer) return std_logic_vector is
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

begin

-- Process
	
	-- Check Sel
	process(SW(6),SW(7)) is
	begin
		Sel <= SW(7) & SW(6);
	end process;
	
	-- Check Speed
	process(Speed) is
	begin
		if Speed ='0' then
			if Counter < 4 then
				Counter <= Counter * 2;
			else
				Counter <= 1;
			end if;
		end if;
			
	end process;
	
	-- Main code (Shift)
	process(Sel, SW, BTN, Reset) is
	begin 
	
	-- Reset BTN
	if Reset = '0' then 
		Output  <= "0000";
	elsif rising_edge(BTN) then
		if Counter = 1 then
		
		case (Sel) is
		when "00" => NULL;
		when "01" => Output <= Output(2 downto 0) & SW(0);
		when "10" => Output <= SW(5) & Output(3 downto 1);
		when "11" => Output <= SW(4 downto 1);
		end case;
		elsif Counter = 2 then
		
		case (Sel) is
		when "00" => NULL;
		when "01" => Output <= Output(1 downto 0) & SW(0) & SW(0);
		when "10" => Output <= SW(5) & SW(5) & Output(3 downto 2);
		when "11" => Output <= SW(4 downto 1);
		end case;
		
		
		elsif Counter = 4 then
		case (Sel) is
		when "00" => NULL;
		when "01" => Output <= SW(0) & SW(0) & SW(0) & SW(0);
		when "10" => Output <= SW(5) & SW(5) & SW(5) & SW(5);
		when "11" => Output <= SW(4 downto 1);
		end case;

		
		end if;
	end if;
	end process;
	
	-- Output
	process(Output, Counter)
	begin
		LED  <= Output;
		HEX0 <= SevenSegmentDisplay(Counter);
	end process;
end Behavioral;