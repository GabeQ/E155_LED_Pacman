//pacman toplevel module
//Created by Kai Kaneshina and Gabriel Quiroz
//2017

module e155_pacman(input logic 				clk, reset,
						 input logic 	[3:0]		row,	
						 output logic 	[3:0]		col, LEDS, rowD,
						 input logic				sck,					//from master
					    input logic				mosi,					//from master
						 //input logic	   		start,
					    output logic				miso, freq1,				//to master
						 output logic  [2:0] 	RGB1, RGB2, data,
						 output logic 				LAT, OE, done,
						 output logic 	[2:0] 	RGB1bus);		
						 
	//d is data to which is keypad presses, Q is data received from pi which is ascii char. I am going to make Q an output of the main module to light up an LED on the board.
	logic [7:0] q;
	logic enable;
	logic [2:0] RGB2bus; //RGB1bus
	logic [9:0] memorylocation;
	
	keypad keypad(clk, reset, row, col, LEDS);
	spi_pi_fpga spi_pi_fpga(sck, mosi, miso, reset, LEDS, q);	

	

	always_ff @(posedge sck)
		if (reset) memorylocation <= 0;
		else if (enable) memorylocation <= memorylocation +1;

	//logic [2:0] data; 
	always_comb 
		case(q)
			8'b01010111: begin 
				data = 3'b100; // case for wall
				enable =1;
				end 
						
			8'b01000110: begin 
				data = 3'b010; //food cases, gives odd case aka second bus, makes them red
				enable =1;
				end 
			8'b01001001: begin 
				data = 3'b001; //case for I
				enable = 1;
				end 
			
			8'b01001111: begin 
				data = 3'b111; //case for O
				enable = 1;
				end 
							
			default: begin 
				data = 3'b000;
				enable = 0; 
				end 
		endcase
		
	//mem memory(sck, enable, memorylocation, data, RGB1bus);	// sck
	//mem memory2(sck,enable, memorylocation + 512, data, RGB2bus);
	//	change adr = memorylocation and rd is the output` 

	
	assign freq1 = sck;
	matrix matrix(sck, reset, enable, data, data, RGB1, RGB2, rowD, LAT, OE); // sck
	

		
	//	logic [2:0] test1,test2;
	//matrix matrix(sck,reset,test1,test2, RGB1,RGB2,rowD,LAT, OE);

	//assign done = &rowD;  
	

			
endmodule



