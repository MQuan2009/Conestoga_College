module MTran_Lab6_Verilog_FullAdder(
		
		input  wire Cin,
		input  wire [3:0] A,
		input  wire [3:0] B,
		output reg  [3:0] Sum,
		output reg  CarryOut
);

reg [3:0] Carry;

always @*
begin
    Sum[0] = A[0] ^ B[0] ^ Cin;
    Carry[0] = (A[0] & B[0]) | (B[0] & Cin) | (A[0] & Cin);

    Sum[1] = A[1] ^ B[1] ^ Carry[0];
    Carry[1] = (A[1] & B[1]) | (B[1] & Carry[0]) | (A[1] & Carry[0]);

    Sum[2] = A[2] ^ B[2] ^ Carry[1];
    Carry[2] = (A[2] & B[2]) | (B[2] & Carry[1]) | (A[2] & Carry[1]);

    Sum[3] = A[3] ^ B[3] ^ Carry[2];
    Carry[3] = (A[3] & B[3]) | (B[3] & Carry[2]) | (A[3] & Carry[2]);

    CarryOut = Carry[3];
end

endmodule