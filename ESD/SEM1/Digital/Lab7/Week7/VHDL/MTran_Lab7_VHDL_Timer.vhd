library ieee;
use ieee.std_logic_1164.ALL;

entity MTran_Lab7_VHDL_Timer is
port
(
	-- Inputs
	Clk     : in    std_logic;
	Enable  : in    std_logic;
	
	-- Outputs
	Seconds : inout integer := 0;
	Minutes : inout integer := 0;
	Hours   : inout integer := 0
	
);
end entity;

architecture RTL of MTran_Lab7_VHDL_Timer is

-- Signal Declaration
signal Ticks	            : integer := 0;

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
	variable SecondsFlag      : boolean := false;
	variable MinutesFlag      : boolean := false;
	variable HoursFlag        : boolean := false;
	
	constant MaxSeconds       : integer := 60;
	constant MaxMinutes       : integer := 60;
	constant MaxHours         : integer := 24;

	constant ClockFrequency   : integer := 50000000;
		
	-- Main
	begin
	
	if Enable = '1' then
		if rising_edge(Clk) then
			
			-- Increment Ticks
			Increment(Ticks, ClockFrequency, SecondsFlag);

			-- Increment Seconds whenever Ticks reach Clock Frequency
			if SecondsFlag then
				Increment(Seconds, MaxSeconds, MinutesFlag);
			end if;
			
			-- Increment Minute whenever 60 seconds
			if SecondsFlag and MinutesFlag then
				Increment(Minutes, MaxMinutes, HoursFlag);
			end if;
			
			-- Increment Hours whenever 60 minutes
			if SecondsFlag and MinutesFlag and HoursFlag then 
				Increment(Hours, MaxHours, HoursFlag);
			end if;
		end if;
	end if;
		
	end process;
end architecture;