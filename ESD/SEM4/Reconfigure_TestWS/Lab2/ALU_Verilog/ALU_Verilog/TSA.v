module TSA (
    input wire EN,
    input wire [7:0] DATA,
    output wire [7:0] OUTPUT
);
    assign OUTPUT = EN ? DATA : 8'b00000000;
endmodule
