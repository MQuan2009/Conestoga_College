library ieee;
use ieee.std_logic_1164.all;

entity MTran_Lab10_VHDL_CrosswalkController is
port
(
    -- Inputs 
    BTN     : in std_logic;
    CLOCK_50: in std_logic;
    
    -- Outputs
    RGB_LED : out std_logic_vector (2 downto 0);
    
    LED_R    : out std_logic_vector (2 downto 0);
    LED_W    : out std_logic_vector (1 downto 0);
    
    HEX0     : out std_logic_vector (6 downto 0);
    HEX1     : out std_logic_vector (6 downto 0)
);
end entity;

architecture Behavioral of MTran_Lab10_VHDL_CrosswalkController is

-- Module declaration
component MTran_Lab10_VHDL_Timer is 
port
(
    -- Inputs
    Clk      : in   std_logic;
    Enable   : in   std_logic;
    MaxTime  : in   integer;
    
    -- Outputs
    Seconds  : inout  integer
);
end component;

-- Function
function SevenSegmentDisplay(Number : integer) return std_logic_vector is
begin
    case (Number) is
        when 0   => return "1000000";
        when 1   => return "1111001";
        when 2   => return "0100100";
        when 3   => return "0110000";
        when 4   => return "0011001";
        when 5   => return "0010010";
        when 6   => return "0000010";
        when 7   => return "1111000";
        when 8   => return "0000000";
        when 9   => return "0010000";
        when others => return "1111111";
    end case;
end SevenSegmentDisplay;

-- Signal Declaration
signal Seconds     : integer   := 10;
signal MaxTime     : integer   := 10;
signal Enable      : std_logic := '0';

signal WaitFlag    : std_logic := '0';
signal DoneFlag    : std_logic := '0';

type TrafficState_type is (GREEN, GREENWAIT1, GREENWAIT2, GREENWAIT3,
									YELLOW, YELLOWWAIT1, YELLOWWAIT2, YELLOWWAIT3,
									RED, REDWAIT1, REDWAIT2, REDWAIT3);

signal CurrentState : TrafficState_type  := GREEN;

begin
    
    -- Module instantiate
    module_timer: MTran_Lab10_VHDL_Timer
    port map
    (
        -- Inputs
        Clk     => CLOCK_50,
        Enable  => Enable,
        MaxTime => MaxTime,
        
        -- Outputs
        Seconds => Seconds
    );
    
    -- State Transition Process
    process(CLOCK_50)
    begin
		if rising_edge(CLOCK_50) then
			if BTN = '0' and CurrentState = Green then 
				CurrentState <= GREENWAIT1;
			end if;
				
			case (CurrentState) is
            when GREEN =>
				
					RGB_LED <= "010";
					LED_R   <= "001";
					LED_W   <= "10";
					
					HEX1 <= SevenSegmentDisplay(10);
					HEX0 <= SevenSegmentDisplay(10);
					
            when GREENWAIT1 =>
				
					Enable  <= '1'; -- Enable Timer 
					CurrentState <= GREENWAIT2;
				-- At this clock edge timer have not count
				
				when GREENWAIT2 => 
				
					MaxTime <= 5;
					CurrentState <= GREENWAIT3;
				-- At this clock edge timer have begun counting but havent update the Seconds
					
				when GREENWAIT3 =>
				
					if Seconds = 0 then 
						Enable  <= '0'; -- Disable Timer
						CurrentState <= YELLOW;
					end if;
					
					HEX1 <= SevenSegmentDisplay(Seconds / 10);
					HEX0 <= SevenSegmentDisplay(Seconds mod 10);
					
            when YELLOW =>
					RGB_LED <= "110";
					LED_R   <= "010";
					LED_W   <= "10";
					
					HEX1 <= SevenSegmentDisplay(10);
					HEX0 <= SevenSegmentDisplay(10);

					CurrentState <= YELLOWWAIT1;
					
            when YELLOWWAIT1 =>
					
					Enable  <= '1'; -- Enable Timer 
					CurrentState <= YELLOWWAIT2;
				
				when YELLOWWAIT2 =>
					MaxTime <= 3;
					CurrentState <= YELLOWWAIT3;
					
				when YELLOWWAIT3 =>

					if Seconds = 0 then 				
						Enable  <= '0'; -- Disable Timer
						CurrentState <= Red;
					end if;
					
					HEX1 <= SevenSegmentDisplay(Seconds / 10);
					HEX0 <= SevenSegmentDisplay(Seconds mod 10);					

            when RED =>
					RGB_LED <= "100";
					LED_R   <= "100";
					LED_W   <= "01";

					CurrentState <= REDWAIT1;
					
            when REDWAIT1 =>
					
					Enable  <= '1'; -- Enable Timer 
					CurrentState <= REDWAIT2;
				
				when REDWAIT2 =>
					MaxTime <= 10;
					CurrentState <= REDWAIT3;
					
				when REDWAIT3 =>

					if Seconds = 0 then 				
						Enable  <= '0'; -- Disable Timer
						CurrentState <= GREEN;
					end if;
					
					HEX1 <= SevenSegmentDisplay(Seconds / 10);
					HEX0 <= SevenSegmentDisplay(Seconds mod 10);
			   when others =>
               LED_R   <= "111";
			end case;
		end if;
    end process;

	 

end Behavioral;
