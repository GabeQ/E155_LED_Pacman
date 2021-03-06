//Keypad toplevel module
//Created by Kai Kaneshina and Gabriel Quiroz
//2017

module keypad(input logic clk, reset,
						input logic [3:0]row,	
						output logic [3:0]col, LEDS);
	//create wires
	logic freq, press, en;
	logic [3:0]next_to_display;
	//set desired frequency
	oscillator #(53687) slw(clk, freq);
	//assign pressed var if any input is high
	assign press = row[0] | row[1] | row[2] | row[3];
				
	//MAIN FSM			
	typedef enum logic [2:0] {S0, S1, S2, S3, S4} statetype;
	statetype state, nextstate;

	always_ff @(posedge freq, posedge reset)
		if (reset) state <= S0;
		else			state <= nextstate;

	//NEXT STATE LOGIC
	always_comb
		case (state)
			S0:	if (~press)		nextstate <= S1;	//go to state 1 if no button is pressed
						else			nextstate <= S4;	//go to state 4 if button is pressed
						
			S1:	if (~press) 	nextstate <= S2;	//go to state 2 if no button is pressed
						else			nextstate <= S4;	//go to state 4 if button is pressed
						
			S2:	if (~press)		nextstate <= S3;	//go to state 3 if no button is pressed
						else			nextstate <= S4;	//go to state 4 if button is pressed
						
			S3:	if (~press)		nextstate <= S0;	//go to state 0 if no button is pressed
						else			nextstate <= S4;	//go to state 4 if button is pressed
						
			S4:	if (~press)		nextstate <= S0;	//go to state 0 if no button is pressed
						else			nextstate <= S4;	//stay at same state if button is pressed
						
			default: nextstate <= S0;
		endcase
		
	//OUTPUT LOGIC
	always_comb
		case (state)
			//null state
			S0:	begin
						col = 4'b1000;
						begin
								if (row == 4'b1000)					next_to_display = 4'b1111;//D
								else if (row == 4'b0100)			next_to_display = 4'b1111;//C
								else if (row == 4'b0010)			next_to_display = 4'b1111;//B
								else if (row == 4'b0001)			next_to_display = 4'b1111;//A
								else										next_to_display = 4'b1111;//null
						end
					end
			//null state
			S1:	begin
						col = 4'b0100;
						begin
								if (row == 4'b1000)					next_to_display = 4'b1111;//F
								else if (row == 4'b0100)			next_to_display = 4'b1111;//9
								else if (row == 4'b0010)			next_to_display = 4'b1111;//6
								else if (row == 4'b0001)			next_to_display = 4'b1111;//3
								else										next_to_display = 4'b1111;//null
						end
					end
			S2:	begin
						col = 4'b0010;
						begin
								if (row == 4'b1000)					next_to_display = 4'b1111;//0
								else if (row == 4'b0100)			next_to_display = 4'b1111;//8
								else if (row == 4'b0010)			next_to_display = 4'b0001;//5
								else if (row == 4'b0001)			next_to_display = 4'b1111;//2
								else										next_to_display = 4'b1111;//null
						end
					end
			S3:	begin
						col = 4'b0001;
						begin
								if (row == 4'b1000)					next_to_display = 4'b1111;//E
								else if (row == 4'b0100)			next_to_display = 4'b0010;//7
								else if (row == 4'b0010)			next_to_display = 4'b0100;//4
								else if (row == 4'b0001)			next_to_display = 4'b1000;//1
								else										next_to_display = 4'b1111;//null
						end
					end
					
					
					
			S4:	begin
						col = 4'b1111;
						next_to_display = 4'b1111;	//null
					end
			default:	begin
							col = 4'b1111;
							next_to_display = 4'b1111;	//null
						end
		endcase
	
	assign en = (next_to_display != 4'b1111);
	
	flopenr #(4) flop1(freq, reset, en, next_to_display, LEDS);
	
endmodule


module flopenr #(parameter WIDTH)
						 (input  logic             clk, reset, en,
						  input  logic [WIDTH-1:0] d, 
						  output logic [WIDTH-1:0] q);

	  always_ff @(posedge clk, posedge reset)
		 if (reset)   q <= 0;
		 else if (en) q <= d;
endmodule


module oscillator #(parameter n)
							 (input logic clk,
							  output logic freq);
		logic [31:0] k;
		
		//add n to k to incrament k
		always_ff @(posedge clk)
			k = k + n;
			
		//assign the desired frequency to be the 31st bit of k
		assign freq = k[31];
		
endmodule
	