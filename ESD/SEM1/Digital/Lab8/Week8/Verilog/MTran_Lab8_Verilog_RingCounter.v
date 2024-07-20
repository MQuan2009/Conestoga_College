module MTran_Lab8_Verilog_RingCounter
(
	// Inputs
	input wire CLK,
	input wire Reset,
	input wire Complement,
	
	// Outputs
	output reg [3:0] Q,
	output reg [1:0] StateNumber
	
);

// Signal Declaration
reg [1:0] state;
reg complementMode;
reg [3:0] Output;

// Function Declaration
function [1:0] Increment;
	input [1:0] Counter;
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

//
always @(state, complementMode)
begin
    case(state)
        0		: Output = 4'b1000;
        1		: Output = 4'b0100;
        2		: Output = 4'b0010;
        3		: Output = 4'b0001;
        default: Output = 4'b0000;
    endcase

    StateNumber = state;
end

endmodule



	