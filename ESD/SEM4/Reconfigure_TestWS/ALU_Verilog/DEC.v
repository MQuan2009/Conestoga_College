module DEC (
    input wire EN,
    input wire [7:0] A,
    output wire [7:0] RESULT,
    output wire BORROW
);
    wire [8:0] temp;
    assign temp = EN ? (A - 1'b1) : 9'b000000000;
    assign RESULT = temp[7:0];
    assign BORROW = temp[8];
endmodule
