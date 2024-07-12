module MTran_Lab7_Verilog_Timer

#(
parameter MAXSECONDS      = 60,
parameter MAXMINUTES      = 60,
parameter MAXHOURS        = 24,
parameter CLKFREQUENCY    = 50000000
)

(
    input wire Clk,
    input wire Enable,
    inout reg [31:0] Seconds,
    inout reg [31:0] Minutes,
    inout reg [31:0] Hours
);

// Signal Declaration
reg [31:0] Ticks = 0;

// Main Logic
always @(posedge Clk)
begin
    if (Enable)
	 begin
		// Increment Ticks
      Ticks = Ticks + 1;
		if (Ticks == CLKFREQUENCY - 1)
			begin
				Ticks   = 0;
				Seconds = Seconds + 1;
			end   
		  
		if (Seconds == MAXSECONDS)
			begin
				Seconds = 0;
				Minutes = Minutes + 1;
			end
		
		if (Minutes == MAXMINUTES)
			begin
				Minutes = 0;
				Hours   = Hours + 1;
			end
		
		if (Hours == MAXHOURS)
			begin
				Hours   = 0;
			end
   end
end

endmodule
