module MTran_Lab6_Verilog_Adder(
		input  wire [9:8] mode,
		input  wire [3:0] A,
		input  wire [3:0] B,
		output reg  [7:0] LED,
		output reg  [6:0] HEX1,
		output reg  [6:0] HEX0
);

/********************Variable and Signal Declaration**************************/
wire [3:0] Sum_HA;
wire [3:0] Carry_HA;

wire [3:0] Difference_HS;
wire [3:0] Borrow_HS;

wire [3:0] Sum_FA;
reg Cin;
wire Carry_FA;

reg [4:0] TotalSum;
reg [4:0] tenth;
reg [4:0] units;
/********************End of Variable and Signal Declaration*******************/

/********************Module Instantiation*************************************/

MTran_Lab6_Verilog_HalfAdder HA (
		.A(A),
		.B(B),
		.Sum(Sum_HA),
		.Carry(Carry_HA)
);

MTran_Lab6_Verilog_HalfSubtractor HS (
		.A(A),
		.B(B),
		.Difference(Difference_HS),
		.Borrow(Borrow_HS)
);

MTran_Lab6_Verilog_FullAdder FA (
		.A(A),
		.B(B),
		.Cin(Cin),
		.Sum(Sum_FA),
		.CarryOut(Carry_FA)
);	
/********************End of Module Instantiation******************************/

/********************Function Declaration*************************************/

function [6:0] SevenSegmentDisplay;
	input reg [4:0] Num;
	begin
		case(Num)
			5'd0: SevenSegmentDisplay = 7'b1000000;
         5'd1: SevenSegmentDisplay = 7'b1111001;
         5'd2: SevenSegmentDisplay = 7'b0100100;
         5'd3: SevenSegmentDisplay = 7'b0110000;
         5'd4: SevenSegmentDisplay = 7'b0011001;
         5'd5: SevenSegmentDisplay = 7'b0010010;
         5'd6: SevenSegmentDisplay = 7'b0000010;
         5'd7: SevenSegmentDisplay = 7'b1111000;
         5'd8: SevenSegmentDisplay = 7'b0000000;
         5'd9: SevenSegmentDisplay = 7'b0010000;
			default: SevenSegmentDisplay = 7'b1111111;
		endcase;
	end
endfunction

/********************End of Function Declaration******************************/
// Main code
always@(mode,A,B)
begin
   // Reset LED and 7 Segment
	HEX1 = 7'b1111111;
	HEX0 = 7'b1111111;
	LED  = 8'b000000000;
	
	// Change mode
	if (mode == 2'b00)
		begin
			LED[3:0] = Sum_HA;
			LED[7:4] = Carry_HA;
		end
	else if (mode == 2'b01)
		begin
			LED[3:0] = Difference_HS;
			LED[7:4] = Borrow_HS;
		end
	else if (mode == 2'b10)
		begin
			Cin = 1'b0;
			
			LED[3:0] = Sum_FA;
			LED[7:4] = Carry_FA;
			
			TotalSum = {Carry_FA, Sum_FA};
			tenth = TotalSum / 10;
			units = TotalSum % 10;
			
			HEX1 = SevenSegmentDisplay(tenth);
			HEX0 = SevenSegmentDisplay(units);
		end
	else if (mode == 2'b11)
		begin
			Cin = 1'b1;
			
			LED[3:0] = Sum_FA;
			LED[7:4] = Carry_FA;
			
			TotalSum = {Carry_FA, Sum_FA};
			tenth = TotalSum / 10;
			units = TotalSum % 10;
			
			HEX1 = SevenSegmentDisplay(tenth);
			HEX0 = SevenSegmentDisplay(units);
		end
		
end 
endmodule