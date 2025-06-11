module SHR (
    input  wire        EN,
    input  wire [7:0]  A,
    output wire [7:0]  RESULT,
    output wire        CARRY
);
    assign CARRY = EN ? A[0] : 1'b0;
    assign RESULT = EN ? {1'b0, A[7:1]} : 8'b00000000;

endmodule
