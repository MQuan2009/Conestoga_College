module MTran_Lab4_Verilog_7SegmentLed (
    input [9:0] SW,
    input [3:0] BT,
    output reg [7:0] LED,
    output reg [6:0] HEX0,
    output reg [6:0] HEX1,
    output reg [6:0] HEX2,
    output reg [6:0] HEX3
);

    // Variable declaration
    reg add_flag, num1_flag, num2_flag;
    integer hundred, tenth, unit;
    integer err, err_num1, err_num2, digit0, digit1, digit2, digit3, num1, num2, result;

    initial
	 begin
        add_flag = 0; 
		  num1_flag = 0;
		  num2_flag = 0;
        hundred = 0;
        tenth = 0;
        unit = 0;
        err = 0;
        err_num1 = 0;
		  err_num2 = 0;
        digit0 = 0;
        digit1 = 0;
        digit2 = 0;
        digit3 = 0;
        num1 = 0;
        num2 = 0;
        result = 0;
    end

    // Function Declaration
	 
	 // Displaying 7 segment function
    function [6:0] SevenSegmentDisplay;
        input [3:0] Switch;
        begin
            case (Switch)
                4'b0000: SevenSegmentDisplay = 7'b1000000;
                4'b0001: SevenSegmentDisplay = 7'b1111001;
                4'b0010: SevenSegmentDisplay = 7'b0100100;
                4'b0011: SevenSegmentDisplay = 7'b0110000;
                4'b0100: SevenSegmentDisplay = 7'b0011001;
                4'b0101: SevenSegmentDisplay = 7'b0010010;
                4'b0110: SevenSegmentDisplay = 7'b0000010;
                4'b0111: SevenSegmentDisplay = 7'b1111000;
                4'b1000: SevenSegmentDisplay = 7'b0000000;
                4'b1001: SevenSegmentDisplay = 7'b0010000;
                4'b1010: SevenSegmentDisplay = 7'b0001000;
                4'b1011: SevenSegmentDisplay = 7'b0000011;
                4'b1100: SevenSegmentDisplay = 7'b1000110;
                4'b1101: SevenSegmentDisplay = 7'b0100001;
                4'b1110: SevenSegmentDisplay = 7'b0000110;
                4'b1111: SevenSegmentDisplay = 7'b0001110;
                default: SevenSegmentDisplay = 7'b1111111;
            endcase
        end
    endfunction

    // Function to display a digit on 7-segment display
    function [6:0] DigitDisplay;
        input [3:0] Switch;
        input integer dig;
        begin
            if (dig <= 9)
					begin
						 DigitDisplay = SevenSegmentDisplay(Switch);
					end 
				else 
					begin
						 err = err + 1;
						 DigitDisplay = SevenSegmentDisplay(4'b1111);
					end
        end
    endfunction

    // Main code
	 
	 // 
    always @(SW) 
	 
	 begin
        // Reset 
        err = 0;
        
        // SW9 to change different assignment
        if (SW[9] == 1'b0) 
		  begin
            HEX3 = 7'b1111111;
            HEX2 = 7'b1111111;
            HEX1 = 7'b1111111;
            HEX0 = SevenSegmentDisplay(SW[3:0]);
        end 
		  
		  else 
		  begin
            // Displaying number 1 and number 2
            if (!add_flag)
				begin
                if (!num1_flag)
					 begin
                    digit0 = SW[3:0];
                    HEX0 = DigitDisplay(SW[3:0], digit0);
                    
                    digit1 = SW[7:4];
                    HEX1 = DigitDisplay(SW[7:4], digit1);
                    
                    // Turn off HEX2 and HEX3
                    HEX2 = 7'b1111111;
                    HEX3 = 7'b1111111;
                end 
					 else
					 begin
                    digit2 = SW[3:0];
                    HEX2 = DigitDisplay(SW[3:0], digit2);
                    
                    digit3 = SW[7:4];
                    HEX3 = DigitDisplay(SW[7:4], digit3);
                    
                    // Turn off HEX0 and HEX1
                    HEX1 = 7'b1111111;
                    HEX0 = 7'b1111111;
                end
            end
				else
				
				// Add up 2 number then display
			   begin
					result = num1 + num2;
					hundred = result / 100;
					tenth   = (result / 10) % 10;
					unit    = result % 10;
					if (result >= 100)
					begin 
						 HEX3 = 7'b1111111;
						 HEX2 = SevenSegmentDisplay(hundred);
						 HEX1 = SevenSegmentDisplay(tenth);
						 HEX0 = SevenSegmentDisplay(unit);
					end
					else if (result >= 10) 
					begin
						 HEX3 = 7'b1111111;
						 HEX2 = 7'b1111111;
						 HEX1 = SevenSegmentDisplay(tenth);
						 HEX0 = SevenSegmentDisplay(unit);
					end
					else
					begin
						 HEX3 = 7'b1111111;
						 HEX2 = 7'b1111111;
						 HEX1 = 7'b1111111;
						 HEX0 = SevenSegmentDisplay(unit);
					end
					
					// Check if error
					if (err_num1 || err_num2)
					begin
						 HEX3 = 7'b1111111;
						 HEX2 = 7'b0000110; //E
						 HEX1 = 7'b0101111; //r
						 HEX0 = 7'b0101111; //r
					end
			  end
        end
	 end
	 
	// Check BT0
	 always@ (negedge(BT[0]) or negedge(BT[3]))
	 begin
		// Reset
		if (!BT[3])
		begin
			num1_flag = 0;
			err_num1 = 0;
			LED[3:0] = 4'b0000;
		end
		
		// Assigning num1
		if (~BT[0])
		begin
			num1_flag = 1;
         if (err == 0)
				begin
				num1 = digit1 * 10 + digit0;
				end
			else
				begin
				LED[3:0] = 4'b1111;
				err_num1 = 1;
				end
      end
	 end
	 
	 // Check BT1
	 always@ (negedge(BT[1]) or negedge(BT[3]))
	 begin
		// Reset
		if (!BT[3])
		begin
			num2_flag = 0;
			err_num2 = 0;
			LED[7:4] = 4'b0000;
		end
		
		// Assigning num2
		if (!BT[1])
		begin
			num2_flag = 1;
         if (err == 0)
				begin
				num2 = digit3 * 10 + digit2;
				end
			else
				begin
				LED[7:4] = 4'b1111;
				err_num2 = 1;
				end
      end
	 end
    
	always@ (negedge(BT[2]) or negedge(BT[3]))
	begin
      if (!BT[3])
		begin
		add_flag = 0;
		end
		else
		begin
		add_flag = 1;
		end
   end  
	
endmodule
