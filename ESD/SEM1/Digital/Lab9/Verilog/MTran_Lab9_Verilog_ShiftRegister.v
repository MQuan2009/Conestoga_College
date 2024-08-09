module MTran_Lab9_Verilog_ShiftRegister
(
    input wire BTN,
    input wire Speed,
    input wire Reset,
    input wire [7:0] SW,
    output reg [3:0] LED,
    output reg [6:0] HEX0
);

    // Signal Declarations
    reg [3:0] Output;
    reg [1:0] Sel;
    integer Counter = 1;

    // Seven Segment Display function
    function [6:0] SevenSegmentDisplay;
        input integer Number;
        begin
            case (Number)
                0: SevenSegmentDisplay = 7'b1000000;
                1: SevenSegmentDisplay = 7'b1111001;
                2: SevenSegmentDisplay = 7'b0100100;
                3: SevenSegmentDisplay = 7'b0110000;
                4: SevenSegmentDisplay = 7'b0011001;
                5: SevenSegmentDisplay = 7'b0010010;
                6: SevenSegmentDisplay = 7'b0000010;
                7: SevenSegmentDisplay = 7'b1111000;
                8: SevenSegmentDisplay = 7'b0000000;
                9: SevenSegmentDisplay = 7'b0010000;
                default: SevenSegmentDisplay = 7'b1111111;
            endcase
        end
    endfunction

    // Check Sel
    always @(*) 
	 begin
        Sel = {SW[7], SW[6]};
    end

    // Check Speed
    always @(negedge Speed)
	 begin
        if (Speed == 0) 
		  begin
            if (Counter < 4)
                Counter = Counter * 2;
            else
                Counter = 1;
        end
    end

    // Main code (Shift)
    always @(negedge BTN or negedge Reset)
	 begin
        if (Reset == 0)
            Output <= 4'b0000;
        else if (BTN == 0)
		  begin
            if (Counter == 1)
				begin
                case (Sel)
                    2'b00: Output = Output;
                    2'b01: Output = {Output[2:0], SW[0]};
                    2'b10: Output = {SW[5], Output[3:1]};
                    2'b11: Output = SW[4:1];
                endcase
            end
				else if (Counter == 2)
				begin
                case (Sel)
                    2'b00: Output = Output;
                    2'b01: Output = {Output[1:0], SW[0], SW[0]};
                    2'b10: Output = {SW[5], SW[5], Output[3:2]};
                    2'b11: Output = SW[4:1];
                endcase
            end
				else if (Counter == 4)
				begin
                case (Sel)
                    2'b00: Output = Output;
                    2'b01: Output = {SW[0], SW[0], SW[0], SW[0]};
                    2'b10: Output = {SW[5], SW[5], SW[5], SW[5]};
                    2'b11: Output = SW[4:1];
                endcase
            end
        end
    end

    // Output
    always @(*) 
	 begin
        LED  = Output;
        HEX0 = SevenSegmentDisplay(Counter);
    end
endmodule
