library ieee;
use ieee.std_logic_1164.all;  

-- Declaring input and output
entity MTran_Lab3_VHDL_ParkIndicator is
    port 
	 (
        SW : in  std_logic_vector (5 downto 0);
        LED: out std_logic_vector (0 to 1);
		  HEX: out std_logic_vector (6 downto 0)
    );
end MTran_Lab3_VHDL_ParkIndicator;

-- Describing the relationship between output and input
architecture behavioral of MTran_Lab3_VHDL_ParkIndicator is

shared variable free_slot: integer := 0;
begin

	-- Counting free slot in parking 
	process(SW)
	begin
	
	-- Resetting free_slot
	free_slot := 0;
	
	-- Counting free slot in parking 
	for i in SW'range loop
		if SW(i) = '0' then 
			free_slot := free_slot + 1; 
		end if;
	end loop;
	
	-- Displayed number of free slot to 7 segment LED
	case free_slot is
	when 0 => HEX <= "1000000";
	when 1 => HEX <= "1111001";
	when 2 => HEX <= "0100100";
	when 3 => HEX <= "0110000";
	when 4 => HEX <= "0011001";
	when 5 => HEX <= "0010010";
	when 6 => HEX <= "0000010";
	when others => HEX <= "1111111";
	end case;
	
	 -- Displaying LED
	 case(SW) is
	 when "111111" => LED <= "10";
	 when others   => LED <= "01";
	 end case;
	 
	 end process;

end behavioral;
 