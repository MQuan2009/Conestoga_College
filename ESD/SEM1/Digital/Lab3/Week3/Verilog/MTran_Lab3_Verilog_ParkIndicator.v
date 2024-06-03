module MTran_Lab3_Verilog_ParkIndicator(
	input wire [5:0] SW,
	output reg [1:0] LED,
	output reg [6:0] HEX
);
	// declaring a vector to store 7 Segment LED code
	reg [6:0] LED_code[15:0];
	integer i = 0;
	integer free_slot = 0;
	
	// initializing declaration
	initial begin
	LED_code[0]  = 7'h40; //0 // can't use 0x40 have to be 7'h or 7'b,...
	LED_code[1]  = 7'h79; //1
	LED_code[2]  = 7'h24; //2
	LED_code[3]  = 7'h30; //3
	LED_code[4]  = 7'h19; //4
	LED_code[5]  = 7'h12; //5
	LED_code[6]  = 7'h02; //6
	LED_code[7]  = 7'h78; //7
	LED_code[8]  = 7'h00; //8
	LED_code[9]  = 7'h10; //9
	LED_code[10] = 7'h08; //A
	LED_code[11] = 7'h03; //b
	LED_code[12] = 7'h46; //C
	LED_code[13] = 7'h21; //d
	LED_code[14] = 7'h06; //E
	LED_code[15] = 7'h0E; //F
	end
	
	// always block (execute again when there are change in variable)
	always @(*) 
	begin
	
	//Resetting variable free_Slot
	free_slot = 0;
	
	// Check how many free slot remain
	for (i = 0; i < 6; i = i + 1)
	begin
		if (SW[i] == 0) 
		begin
			free_slot = free_slot + 1;
		end
	end 
	
	//Display remaining free slot on 7 segment led
	HEX = LED_code[free_slot];
	
	//Displaying LED
   case (SW)
		6'b111111: LED = 2'b10;
		default:   LED = 2'b01;
	endcase
	end
endmodule

