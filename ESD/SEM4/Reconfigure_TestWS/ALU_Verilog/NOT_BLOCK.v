module NOT_BLOCK (
    input wire EN,
    input wire [7:0] A,
    output wire [7:0] RESULT
);
    assign RESULT = EN ? ~A : 8'b00000000;
endmodule

