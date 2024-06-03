module MTran_Lab5_Verilog_OctalDecoder (
	input EN,
	input [2:0] SW,
	input [1:0] mode,
	output reg [7:0] LED,
	output reg [6:0] HEX
);



// Variable Declaring
reg [7:0] LED_output;

// Function's Declaration

// Function for Octal Decoder
function [7:0] Decoder3to8_NormalMode;
input [2:0] Switch;
begin
	case (Switch)
	3'd0: Decoder3to8_NormalMode = 8'b00000001;
	3'd1: Decoder3to8_NormalMode = 8'b00000010;
	3'd2: Decoder3to8_NormalMode = 8'b00000100;
	3'd3: Decoder3to8_NormalMode = 8'b00001000;
	3'd4: Decoder3to8_NormalMode = 8'b00010000;
	3'd5: Decoder3to8_NormalMode = 8'b00100000;
	3'd6: Decoder3to8_NormalMode = 8'b01000000;
	3'd7: Decoder3to8_NormalMode = 8'b10000000;
	default: Decoder3to8_NormalMode = 8'b00000000;
	endcase
end
endfunction

function [7:0] Decoder3to8_MultilineMode;
input [2:0] Switch;
begin
	case (Switch)
	3'd0: Decoder3to8_MultilineMode = 8'b00000001;
	3'd1: Decoder3to8_MultilineMode = 8'b00000011;
	3'd2: Decoder3to8_MultilineMode = 8'b00000111;
	3'd3: Decoder3to8_MultilineMode = 8'b00001111;
	3'd4: Decoder3to8_MultilineMode = 8'b00011111;
	3'd5: Decoder3to8_MultilineMode = 8'b00111111;
	3'd6: Decoder3to8_MultilineMode = 8'b01111111;
	3'd7: Decoder3to8_MultilineMode = 8'b11111111;
	default: Decoder3to8_MultilineMode = 8'b00000000;
	endcase
end
endfunction

// Seven Segment Display
function [6:0] SevenSegmentDisplay;
input [2:0] Switch;
begin
	case (Switch)
	3'd0: SevenSegmentDisplay = 7'b1000000;
	3'd1: SevenSegmentDisplay = 7'b1111001;
	3'd2: SevenSegmentDisplay = 7'b0100100;
	3'd3: SevenSegmentDisplay = 7'b0110000;
	3'd4: SevenSegmentDisplay = 7'b0011001;
	3'd5: SevenSegmentDisplay = 7'b0010010;
	3'd6: SevenSegmentDisplay = 7'b0000010;
	3'd7: SevenSegmentDisplay = 7'b1111000;
	default: SevenSegmentDisplay = 7'b1111111;
	endcase
end
endfunction

// Main code
always @(SW,EN,mode) 
begin
	if (EN)
		begin
			case (mode)
			
			// Normal Mode
			2'b00:
				begin
					LED = Decoder3to8_NormalMode(SW);
					HEX = SevenSegmentDisplay(SW);
				end

			// Complement Mode
			2'b01:
				begin
					LED_output = Decoder3to8_NormalMode(SW);
					HEX = SevenSegmentDisplay(SW);
					LED = ~LED_output;
				end

			// Multiline Mode
			2'b10:
				begin
					LED = Decoder3to8_MultilineMode(SW);
					HEX = SevenSegmentDisplay(SW);
				end

			// Complement Multiline Mode
			2'b11:
				begin
					LED_output = Decoder3to8_MultilineMode(SW);
					HEX = SevenSegmentDisplay(SW);
					LED = ~LED_output;
				end
			endcase
		end 
	else 
	begin
		LED = 8'b00000000;
		HEX = 7'b1111111;
	end
end

endmodule