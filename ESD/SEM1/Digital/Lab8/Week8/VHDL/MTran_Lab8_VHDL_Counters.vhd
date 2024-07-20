library ieee;
use ieee.std_logic_1164.ALL;

entity MTran_Lab8_VHDL_Counters is
port
(
	-- Inputs
	BTN         : in std_logic;
	Reset       : in std_logic;
	Complement 	: in std_logic;
	
	-- Outputs
	LED_Ring 	: out std_logic_vector (3 downto 0);
	LED_Johnson : out std_logic_vector (3 downto 0);
	
	HEX0        : out std_logic_vector (6 downto 0);
	HEX4        : out std_logic_vector (6 downto 0)
	
);
end entity;

architecture Behavioral of MTran_Lab8_VHDL_Counters is

--Signal Declaration
signal state: integer:= 0;

signal RingCounterState   : integer := 0;
signal JohnsonCounterState: integer := 0;

-- Module Declaration
component MTran_Lab8_VHDL_RingCounter is
port
(
	-- Inputs
	CLK    		: in 	std_logic;
	Reset  		: in  std_logic;
	Complement 	: in  std_logic;
	
	-- Outputs
	Q		 		: out std_logic_vector (3 downto 0);
	stateNumber : out integer
);
end component;

component MTran_Lab8_VHDL_JohnsonCounter is
port
(
	-- Inputs
	CLK    		: in  std_logic;
	Reset  		: in  std_logic;
	Complement 	: in  std_logic;
	
	-- Outputs
	Q		 		: out std_logic_vector (3 downto 0);
	stateNumber : out integer
	
);
end component;

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
	-- Module Institiate
	module_RingCounter: MTran_Lab8_VHDL_RingCounter
	port map
	(
		-- Inputs
		CLK         => BTN ,
		Reset  	   => Reset,
		Complement  => Complement,
		
		-- Output
		Q		 	   => LED_Ring,
		stateNumber => RingCounterState
	);
	
	module_JohnsonCounter: MTran_Lab8_VHDL_JohnsonCounter
	port map
	(
		-- Inputs
		CLK   		=> BTN ,
		Reset  		=> Reset,
		Complement 	=> Complement,
		
		-- Output
		Q		    => LED_Johnson,
		stateNumber => JohnsonCounterState
	);
	
	-- Process
	process(RingCounterState, JohnsonCounterState) is
	begin
		HEX0 <= SevenSegmentDisplay(RingCounterState);
		HEX4 <= SevenSegmentDisplay(JohnsonCounterState);
	end process;
	
end Behavioral;