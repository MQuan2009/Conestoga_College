library ieee;
use ieee.std_logic_1164.all;  
use ieee.numeric_std.all;
-- Declaring input and output
entity MTran_Lab4_VHDL_7SegmentLed is
    port 
     (
        SW :  in  std_logic_vector (9 downto 0);
		  BT :  in  std_logic_vector (3 downto 0);
        LED:  out std_logic_vector (7 downto 0);
		  HEX0: out std_logic_vector (6 downto 0);
		  HEX1: out std_logic_vector (6 downto 0);
		  HEX2: out std_logic_vector (6 downto 0);
		  HEX3: out std_logic_vector (6 downto 0)

		);
end MTran_Lab4_VHDL_7SegmentLed;

-- Describing the relationship between output and input
architecture behavioral of MTran_Lab4_VHDL_7SegmentLed is
shared variable num1_flag, num2_flag, add_flag, err_num1, err_num2: Boolean := false;
shared variable hundred, tenth, unit: integer := 0;
shared variable err, digit0, digit1, digit2, digit3, num1, num2, result: integer := 0;

-- Function declaration

-- Assignment
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
 
-- Displaying Digit --myfunction(int a, int b)
function DigitDisplay(Switch : std_logic_vector (3 downto 0);dig : integer) return std_logic_vector is
begin
	if (dig <= 9) then return SevenSegmentDisplay(Switch);
	else 
	return SevenSegmentDisplay("1111");
	end if;

end DigitDisplay;

-- Check Digit
function DigitCheck(dig0 : integer; dig1 : integer) return Boolean is
begin
	if (dig0 > 9 or dig1 > 9) then 
	return true;
	else return false;
	end if;

end DigitCheck;

-- End of Function Declaration

-- Main code
begin
	
	-- Check SW
	Control_Number: process(SW,BT(3))
	begin
	
	-- Reset 
	err := 0;
	if (BT(3) = '0') then
		HEX3 <= "1111111";
		HEX2 <= "1111111";
		HEX1 <= "1111111";
		HEX0 <= "1111111";
		err_num1 := false;
		err_num2 := false;
	end if;
	
	-- SW9 to change different assignment
	if (SW(9) = '0') then
		HEX3 <= "1111111";
		HEX2 <= "1111111";
		HEX1 <= "1111111";
		HEX0 <= SevenSegmentDisplay(SW(3 downto 0)); 
	else 
	
	-- Displaying number 1 and number 2
		if (add_flag = false) then
			if (num1_flag = false) then
				digit0 := to_integer(unsigned(SW(3 downto 0)));
				HEX0 <= DigitDisplay(SW(3 downto 0),digit0);
				
				digit1 := to_integer(unsigned(SW(7 downto 4)));
				HEX1 <= DigitDisplay(SW(7 downto 4),digit1);
				
				-- Check num1
				err_num1 := DigitCheck(digit0, digit1);
				
				-- Turn off HEX2 and 
				HEX2 <= "1111111";
				HEX3 <= "1111111";
			elsif (num2_flag = false) then
				digit2 := to_integer(unsigned(SW(3 downto 0)));
				HEX2 <= DigitDisplay(SW(3 downto 0),digit2);
				
				digit3 := to_integer(unsigned(SW(7 downto 4)));
				HEX3 <= DigitDisplay(SW(7 downto 4),digit3);

				-- Check num2
				err_num2 := DigitCheck(digit3, digit2);
				
				-- Turn off HEX0 and 
				HEX1 <= "1111111";
				HEX0 <= "1111111";
			end if;
		else 
		
			-- Add up 2 number then display
			result := num1 + num2;
			hundred := result / 100;
			tenth   := (result / 10) mod 10;
			unit    := result mod 10;
			if (result >= 100) then 
				HEX3 <= "1111111";
				HEX2 <= SevenSegmentDisplay(std_logic_vector(to_unsigned(hundred,4)));
				HEX1 <= SevenSegmentDisplay(std_logic_vector(to_unsigned(tenth,4)));
				HEX0 <= SevenSegmentDisplay(std_logic_vector(to_unsigned(unit,4)));
			elsif result >= 10 then
				HEX3 <= "1111111";
				HEX2 <= "1111111";
				HEX1 <= SevenSegmentDisplay(std_logic_vector(to_unsigned(tenth,4)));
				HEX0 <= SevenSegmentDisplay(std_logic_vector(to_unsigned(unit,4)));
			else
				HEX3 <= "1111111";
				HEX2 <= "1111111";
				HEX1 <= "1111111";
				HEX0 <= SevenSegmentDisplay(std_logic_vector(to_unsigned(unit,4)));
			end if;
			
			-- Check if error
			if (err_num1 or err_num2) then
				HEX3 <= "1111111";
				HEX2 <= "0000110"; --E
				HEX1 <= "0101111"; --r
				HEX0 <= "0101111"; --r
			end if;
		end if;
	end if;
	end process Control_Number;
	
	-- BT0 to confirm number
	Check_BT0: process (BT(0),BT(3))
	begin
		if (BT(3) = '0') then
			LED (3 downto 0) <= "0000";
			num1_flag := false;
		end if;
		
		if (BT(0) = '0') then
			if (num1_flag = false) then
				num1_flag := true;
				if not err_num1 then
					num1 := digit1 * 10 + digit0;
				else 
					LED (3 downto 0) <= "1111";
				end if;
			end if;
		end if;
	end process Check_BT0;

	-- BT1 to confirm number 2
	Check_BT1: process (BT(1),BT(3))
	begin
		if (BT(3) = '0') then
			LED (7 downto 4) <= "0000";
			num2_flag := false;
		end if;
		
		if (BT(1) = '0') then
			if (num2_flag = false) then
				num2_flag := true;
				if not err_num2 then
					num2 := digit3 * 10 + digit2;
				else 
					LED (7 downto 4) <= "1111";
				end if;
			end if;
		end if;
	end process Check_BT1;
	
	-- Check BT2
	Calculate_BT2: process (BT(2), BT(3))
	begin
		if BT(3) = '0' then
			add_flag := false;
		end if;
		if BT(2) = '0' then 
			add_flag := true;
		end if;
	end process Calculate_BT2;

end behavioral;
