
//pacman toplevel module
//Created by Kai Kaneshina and Gabriel Quiroz
//2017

module e155_pacman(input logic 				clk, reset,
						 input logic 	[3:0]		row,	
						 output logic 	[3:0]		col, LEDS, rowD,
						 input logic				sck,					//from master
					    input logic				mosi,					//from master
					    output logic				miso, freq1,				//to master
						 output logic  [2:0] 	RGB1, RGB2,
						 output logic 				LAT, OE, enable, memoryclock);		
						 
	//d is data to which is keypad presses, Q is data received from pi which is ascii char. I am going to make Q an output of the main module to light up an LED on the board.
	logic [7:0] q;
	//logic enable;
	logic [2:0] cnt; //RGB1bus
	logic [8:0] memorylocation;
	logic [5:0] RGBbus; 
	assign enable = (cnt==3'b110);
	
	assign freq1 = &cnt; 
	assign memoryclock = (cnt==3'b000);
	keypad keypad(clk, reset, row, col, LEDS); // should be clk
	spi_pi_fpga spi_pi_fpga(sck, mosi, miso, reset, LEDS, q,cnt);	

	
	//assign enable = &cnt && sck;
		
	always_ff @(negedge enable) //finished 
		if (reset) memorylocation <= 0;
		else memorylocation <= memorylocation +1;
	logic [5:0] newQ;
	assign newQ = (q[6:4]<<3) | q[2:0];
	mem memory(freq1,memoryclock, clk, memorylocation, newQ, RGBbus);	

	matrix matrix(freq1, reset, RGBbus[5:3], RGBbus[2:0], RGB1, RGB2, rowD, LAT, OE);

	//assign done = &rowD;  
	

			
endmodule



