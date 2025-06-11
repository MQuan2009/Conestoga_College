module REG_MUX(
    input wire [63:0] REGS,
    input wire [2:0] SEL_A,
    input wire [2:0] SEL_B,
    output wire [7:0] Abus,
    output wire [7:0] Bbus
);
    assign Abus = REGS[{SEL_A, 3'b000} +: 8];
    assign Bbus = REGS[{SEL_B, 3'b000} +: 8];
endmodule
