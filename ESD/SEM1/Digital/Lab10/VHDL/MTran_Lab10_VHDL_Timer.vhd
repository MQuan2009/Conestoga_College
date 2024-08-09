library ieee;
use ieee.std_logic_1164.ALL;

entity MTran_Lab10_VHDL_Timer is
port
(
	-- Inputs
	Clk      : in  std_logic;
	Enable   : in  std_logic;
	MaxTime  : in 	integer;
	
	-- Outputs
	Seconds  : inout integer
	
);
end entity;

architecture RTL of MTran_Lab10_VHDL_Timer is

-- Signal Declaration
signal   Ticks 			: integer := 0;
constant ClockFrequency : integer := 50000000;

-- Procedure
procedure Increment( signal   Counter   : inout integer;
							constant MaxValue  : in    integer;
							variable ResetFlag : out   boolean) is
begin
	if Counter = MaxValue - 1 then
		-- Reset Counter
		Counter  <= 0;
		ResetFlag := True;
	else
		Counter <= Counter + 1;
		ResetFlag := False;
	end if;
end procedure;

begin	
	process(Clk, Enable) is
	
	-- Variable Declaring
	variable TicksFlag      : boolean := false;

	-- Main
	begin
	
	if Enable = '1' then
		if rising_edge(Clk) then
			
			-- Increment Ticks
			Increment(Ticks, ClockFrequency, TicksFlag);

			-- Increment Seconds whenever Ticks reach Clock Frequency
			if TicksFlag then
				if Seconds > 0 then
					Seconds <= Seconds - 1;
				end if;
			elsif not TicksFlag and Seconds = 0 then
				Seconds <= MaxTime;
			end if;

		end if;
	end if;
		
	end process;
end architecture;