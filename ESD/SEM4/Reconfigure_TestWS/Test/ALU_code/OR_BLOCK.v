module OR_BLOCK (
    input wire EN,
    input wire [7:0] A,
    input wire [7:0] B,
    output wire [7:0] RESULT
);
    assign RESULT = EN ? (A | B) : 8'b00000000;
endmodule
