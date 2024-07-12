module MTran_Lab7_Verilog_MagnitudeComparator 
(
    input wire CLOCK_50,
    input wire [3:0] A,
    input wire [3:0] B,
    input mode,
	 input speed,
    output reg [2:0] LED,
    output reg [6:0] HEX0,
    output reg [6:0] HEX1,
    output reg [6:0] HEX2,
    output reg [6:0] HEX3,
    output reg [6:0] HEX4,
    output reg [6:0] HEX5
);

// Signals declaration
reg [31:0] intA;
reg [31:0] intB;

reg Enable;

wire [31:0] Seconds1;
wire [31:0] Minutes1;
wire [31:0] Hours1;

wire [31:0] Seconds2;
wire [31:0] Minutes2;
wire [31:0] Hours2;

// Module Instantiation
MTran_Lab7_Verilog_Timer
#(
	 .MAXSECONDS(60),
	 .MAXMINUTES(60),
	 .MAXHOURS(24),
	 .CLKFREQUENCY(500000) // 500kHz => Clock 10 times faster
)

Tim1
(
    .Clk(CLOCK_50),
    .Enable(Enable),
    .Seconds(Seconds1),
    .Minutes(Minutes1),
    .Hours(Hours1)
);

MTran_Lab7_Verilog_Timer
#(
	 .MAXSECONDS(60),
	 .MAXMINUTES(60),
	 .MAXHOURS(24),
	 .CLKFREQUENCY(50000000) // 50 MHz => Normal Clk
)
Tim2
(
    .Clk(CLOCK_50),
    .Enable(Enable),
    .Seconds(Seconds2),
    .Minutes(Minutes2),
    .Hours(Hours2)
);

// Function for Seven Segment Display
function [6:0] SevenSegmentDisplay;
    input [3:0] Number;
    begin
        case (Number)
            4'h0: SevenSegmentDisplay = 7'b1000000;
            4'h1: SevenSegmentDisplay = 7'b1111001;
            4'h2: SevenSegmentDisplay = 7'b0100100;
            4'h3: SevenSegmentDisplay = 7'b0110000;
            4'h4: SevenSegmentDisplay = 7'b0011001;
            4'h5: SevenSegmentDisplay = 7'b0010010;
            4'h6: SevenSegmentDisplay = 7'b0000010;
            4'h7: SevenSegmentDisplay = 7'b1111000;
            4'h8: SevenSegmentDisplay = 7'b0000000;
            4'h9: SevenSegmentDisplay = 7'b0010000;
            default: SevenSegmentDisplay = 7'b1111111;
        endcase
    end
endfunction



always @(posedge CLOCK_50) 
begin
    if (mode == 1'b0) 
	 begin
        // Disable Timer
        Enable = 1'b0;

        // Reset HEX2
        HEX2 = 7'b1111111;

        // Convert A and B from std_logic to integer
        intA = A;
        intB = B;

        // Display A and B in decimal
        // A
        HEX0 = SevenSegmentDisplay(intA % 10);
        HEX1 = SevenSegmentDisplay(intA / 10);

        // B
        HEX4 = SevenSegmentDisplay(intB % 10);
        HEX5 = SevenSegmentDisplay(intB / 10);

        // Compare A and B
        if (intA > intB) 
			  begin
					LED  = 3'b100;
					HEX3 = 7'b0100111;
			  end 
		  else if (intA == intB) 
			  begin
					LED  = 3'b010;
					HEX3 = 7'b0110111;
			  end 
		  else
			  begin
					LED  = 3'b001;
					HEX3 = 7'b0110011;
			  end

    end 
	 else
	 begin
        Enable = 1'b1;
        LED    = 3'b000;
		  
		  if (speed == 1'b0) //normal mode
		  begin
			  // Display Second
			  HEX0  = SevenSegmentDisplay(Seconds2 % 10);
			  HEX1  = SevenSegmentDisplay(Seconds2 / 10);

			  // Display Minute
			  HEX2  = SevenSegmentDisplay(Minutes2 % 10);
			  HEX3  = SevenSegmentDisplay(Minutes2 / 10);

			  // Display Hour
			  HEX4  = SevenSegmentDisplay(Hours2 % 10);
			  HEX5  = SevenSegmentDisplay(Hours2 / 10);
			end
			else if (speed == 1'b1)
			begin
				// Display Second
			  HEX0  = SevenSegmentDisplay(Seconds1 % 10);
			  HEX1  = SevenSegmentDisplay(Seconds1 / 10);

			  // Display Minute
			  HEX2  = SevenSegmentDisplay(Minutes1 % 10);
			  HEX3  = SevenSegmentDisplay(Minutes1 / 10);

			  // Display Hour
			  HEX4  = SevenSegmentDisplay(Hours1 % 10);
			  HEX5  = SevenSegmentDisplay(Hours1 / 10);
			end
    end
end

endmodule
