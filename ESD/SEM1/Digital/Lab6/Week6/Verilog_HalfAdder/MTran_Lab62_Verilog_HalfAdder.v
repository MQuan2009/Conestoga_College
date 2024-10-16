module MTran_Lab62_Verilog_HalfAdder(
    input mode,
    input [3:0] A,
    input [7:4] B,
    output reg [3:0] Sum,
    output reg [7:4] Carry,
    output reg [6:0] HEX1,
    output reg [6:0] HEX0
);

// Function to calculate Sum of Half Adder
function [3:0] Sum_HalfAdder(input [3:0] a, input [3:0] b);
begin
    Sum_HalfAdder = a ^ b;
end
endfunction

// Function to calculate Carry of Half Adder
function [3:0] Carry_HalfAdder(input [3:0] a, input [3:0] b);
begin
    Carry_HalfAdder = a & b;
end
endfunction

// Function to calculate Difference of Half Subtractor
function [3:0] Difference_HalfSubtractor(input [3:0] a, input [3:0] b);
begin
    Difference_HalfSubtractor = a ^ b;
end
endfunction

// Function to calculate Borrow of Half Subtractor
function [3:0] Borrow_HalfSubtractor(input [3:0] a, input [3:0] b);
begin
    Borrow_HalfSubtractor = (~a) & b;
end
endfunction

// Function for Seven Segment Display
function [6:0] SevenSegmentDisplay(input [3:0] Switch);
begin
    case (Switch)
        4'b0000: SevenSegmentDisplay = 7'b1000000;
        4'b0001: SevenSegmentDisplay = 7'b1111001;
        4'b0010: SevenSegmentDisplay = 7'b0100100;
        4'b0011: SevenSegmentDisplay = 7'b0110000;
        4'b0100: SevenSegmentDisplay = 7'b0011001;
        4'b0101: SevenSegmentDisplay = 7'b0010010;
        4'b0110: SevenSegmentDisplay = 7'b0000010;
        4'b0111: SevenSegmentDisplay = 7'b1111000;
        4'b1000: SevenSegmentDisplay = 7'b0000000;
        4'b1001: SevenSegmentDisplay = 7'b0010000;
        4'b1010: SevenSegmentDisplay = 7'b0001000;
        4'b1011: SevenSegmentDisplay = 7'b0000011;
        4'b1100: SevenSegmentDisplay = 7'b1000110;
        4'b1101: SevenSegmentDisplay = 7'b0100001;
        4'b1110: SevenSegmentDisplay = 7'b0000110;
        4'b1111: SevenSegmentDisplay = 7'b0001110;
        default: SevenSegmentDisplay = 7'b1111111;
    endcase
end
endfunction

// Main code
always @(*) begin
    if (mode == 1'b0) begin
        Sum = Sum_HalfAdder(A, B);
        HEX0 = SevenSegmentDisplay(Sum_HalfAdder(A, B));
        Carry = Carry_HalfAdder(A, B);
        HEX1 = SevenSegmentDisplay(Carry_HalfAdder(A, B));
    end else if (mode == 1'b1) begin
        Sum = Difference_HalfSubtractor(A, B);
        HEX0 = SevenSegmentDisplay(Difference_HalfSubtractor(A, B));
        Carry = Borrow_HalfSubtractor(A, B);
        HEX1 = SevenSegmentDisplay(Borrow_HalfSubtractor(A, B));
    end
end

endmodule
