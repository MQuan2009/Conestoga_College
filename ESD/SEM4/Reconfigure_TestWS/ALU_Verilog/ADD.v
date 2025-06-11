module ADD (
    input wire EN,
    input wire CARRYIN,
    input wire [7:0] A,
    input wire [7:0] B,
    output wire [7:0] RESULT,
    output wire CARRYOUT
);
    wire [8:0] temp;
    assign temp = EN ? (A + B + CARRYIN) : 9'b000000000;
    assign RESULT = temp[7:0];
    assign CARRYOUT = temp[8];
endmodule

