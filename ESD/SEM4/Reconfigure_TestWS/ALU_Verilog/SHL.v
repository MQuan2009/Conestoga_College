module SHL (
    input  wire        EN,
    input  wire [7:0]  A,
    output wire [7:0]  RESULT,
    output wire        CARRY
);
    assign CARRY = EN ? A[7] : 1'b0;
    assign RESULT = EN ? {A[6:0], 1'b0} : 8'b00000000;

endmodule
