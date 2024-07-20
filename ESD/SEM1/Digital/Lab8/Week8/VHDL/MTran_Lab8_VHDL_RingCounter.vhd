library ieee;
use ieee.std_logic_1164.all;

entity MTran_Lab8_VHDL_RingCounter is
port
(
	-- Inputs
	CLK          : in    std_logic;
	Reset        : in    std_logic;
	Complement 	 : in    std_logic;
	
	-- Outputs
	Q       		 : out 	std_logic_vector (3 downto 0);
	stateNumber  : out   integer
	
);
end entity;

architecture RTL of MTran_Lab8_VHDL_RingCounter is
-- Signal Declaration
signal state			 : integer   := 0;
signal complementMode : std_logic := '0';

signal Output         : std_logic_vector (3 downto 0) := "0000";

-- Procedure
procedure Increment (signal Counter: inout integer) is
begin
	if Counter < 3 then
		Counter <= Counter + 1;
	else 
		Counter <= 0;
	end if;
end procedure;
							

-- Main code
begin
	
	-- Process
	process(CLK,Reset)
	begin
		if Reset = '0' then
			state <= 0;
		else 
			if rising_edge(CLK) then
				Increment(state);
			end if;
		end if;
	end process;
	
	process(state, complementMode)
	begin
		case(state) is
			when 0 		=> Output <= "1000";
			when 1 		=> Output <= "0100";
			when 2 		=> Output <= "0010";
			when 3 		=> Output <= "0001";
			when others => Output <= "0000";
		end case;
		
		stateNumber <= state;
	end process;
	
	-- Check BTN1 for complement mode
	process(Complement)
	begin
		if rising_edge(Complement) then
			complementMode <= not complementMode;
		end if;
	end process;
	
	-- Process complement mode
	process (complementMode) is
	begin
		if complementMode = '1' then
			Q <= not Output;
		else 
			Q <= Output;
		end if;
	end process;
	
end RTL;