module MTran_Lab8_Verilog_JohnsonCounter
(
	// Inputs
	input wire CLK,
	input wire Reset,
	input wire Complement,
	
	// Outputs
	output reg [3:0] Q,
	output reg [2:0] StateNumber
	
);

// Signal Declaration
reg [2:0] state;
reg complementMode;
reg [3:0] Output;

// Function Declaration
function [2:0] Increment;
	input [2:0] Counter;
	begin
	if (Counter < 7)
		begin
			Increment = Increment + 1;
		end
	else 
		begin
			Increment = 0;
		end
	end 
endfunction

// Process

// Check CLK and Reset
always @ (negedge CLK or negedge Reset)
begin
	if (Reset == 0)
		begin
			state = 0;
		end
	else
		begin
			if (CLK == 0)
			begin
				state = Increment(state);
			end
		end
end

// Check Complement
always @ (negedge Complement)
begin
	if (Complement == 0)
	begin
		complementMode = ~complementMode;
	end
end

always @ (complementMode)
begin
	if (complementMode == 1)
		begin
			Q = ~Output;
		end
	else
		begin
			Q = Output;
		end
end

// Check State
always @(state, complementMode)
begin
    case(state)
        0		: Output = 4'b0000;
        1		: Output = 4'b1000;
        2		: Output = 4'b1100;
        3		: Output = 4'b1110;
        4		: Output = 4'b1111;
        5		: Output = 4'b0111;
        6		: Output = 4'b0011;
        7		: Output = 4'b0001;
        default: Output = 4'b0000;
    endcase

    StateNumber = state;
end

endmodule



	