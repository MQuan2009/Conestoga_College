library ieee;
use ieee.std_logic_1164.all;  

-- Declaring input and output
entity MTran_Lab5_VHDL_OctalDecoder is
    port 
	 (
		  EN   : in  std_logic;
        SW   : in  std_logic_vector (2 downto 0);
		  mode : in  std_logic_vector (1 downto 0);
        LED  : out std_logic_vector (7 downto 0);
		  HEX  : out std_logic_vector (6 downto 0)
    );
end MTran_Lab5_VHDL_OctalDecoder;

-- Describing the relationship between output and input
architecture behavioral of MTran_Lab5_VHDL_OctalDecoder is

-- Variable Declaring
signal LED_output : std_logic_vector (7 downto 0) := "00000000";

-- Function's Declaration

-- Function for Octal Decoder
function Decoder3to8_NormalMode(Switch: in std_logic_vector (2 downto 0)) return std_logic_vector is
begin
	case Switch is
	when "000" => return "00000001";
	when "001" => return "00000010";
	when "010" => return "00000100";
	when "011" => return "00001000";
	when "100" => return "00010000";
	when "101" => return "00100000";
	when "110" => return "01000000";
	when "111" => return "10000000";
	end case;
end Decoder3to8_NormalMode;

function Decoder3to8_MultilineMode(Switch: in std_logic_vector (2 downto 0)) return std_logic_vector is
begin
	case Switch is
	when "000" => return "00000001";
	when "001" => return "00000011";
	when "010" => return "00000111";
	when "011" => return "00001111";
	when "100" => return "00011111";
	when "101" => return "00111111";
	when "110" => return "01111111";
	when "111" => return "11111111";
	end case;
end Decoder3to8_MultilineMode;

-- Seven Segment Display
function SevenSegmentDisplay(Switch : std_logic_vector (2 downto 0)) return std_logic_vector is
begin
	case Switch is
	when "000" => return "1000000";
	when "001" => return "1111001";
	when "010" => return "0100100";
	when "011" => return "0110000";
	when "100" => return "0011001";
	when "101" => return "0010010";
	when "110" => return "0000010";
	when "111" => return "1111000";
	end case;
end SevenSegmentDisplay;

begin
	process(SW, mode)
	begin
	if EN = '1' then
		case mode is
		
		-- Normal Mode
		when "00" =>
			LED <= Decoder3to8_NormalMode(SW);
			HEX <= SevenSegmentDisplay(SW);
		
		-- Complement Mode
		when "01" =>
			LED_output <= Decoder3to8_NormalMode(SW);
			HEX <= SevenSegmentDisplay(SW);
			LED <= not LED_output;
		
		-- Multiline Mode
		when "10" =>
			LED <= Decoder3to8_MultilineMode(SW);
			HEX <= SevenSegmentDisplay(SW);
		
		-- Complement Multiline Mode
		when "11" =>
			LED_output <= Decoder3to8_MultilineMode(SW);
			HEX <= SevenSegmentDisplay(SW);
			LED <= not LED_output;
		end case;
	else 
		LED <= "00000000";
		HEX <= "1111111";
	end if;	
	end process;

end behavioral;
 