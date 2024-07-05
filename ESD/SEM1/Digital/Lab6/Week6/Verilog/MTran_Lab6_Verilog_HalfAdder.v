module MTran_Lab6_Verilog_HalfAdder(
		input  wire [3:0] A,
		input  wire [3:0] B,
		output reg [3:0] Sum,
		output reg [3:0] Carry
);
always @*
begin
	Sum = A ^ B;
	Carry = A & B;
end
endmodule