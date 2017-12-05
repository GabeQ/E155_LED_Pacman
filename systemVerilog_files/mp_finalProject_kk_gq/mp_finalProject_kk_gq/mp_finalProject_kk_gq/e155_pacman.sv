
//pacman toplevel module
//Created by Kai Kaneshina and Gabriel Quiroz
//2017

module e155_pacman(input logic 				clk, reset, donesending,
						 input logic 	[3:0]		row,	
						 output logic 	[3:0]		col, LEDS, rowD,
						 input logic				sck,					//from master
					    input logic				mosi,					//from master
					    output logic				miso, freq1, freq2,			//to master
						 output logic  [2:0] 	RGB1, RGB2,
						 output logic 				LAT, OE, enable, we, full);		
						 
	//d is data to which is keypad presses, Q is data received from pi which is ascii char. I am going to make Q an output of the main module to light up an LED on the board.
	logic [7:0] q;
	logic [2:0] cnt; 
	
	
	assign freq1 = &cnt; 
	keypad keypad(clk, reset, row, col, LEDS); // should be clk
	spi_pi_fpga spi_pi_fpga(sck, mosi, miso, reset, LEDS, q,cnt);	


	logic [5:0] newQ;
	assign newQ = (q[6:4]<<3) | q[2:0];
	
		
	
	matrix matrix(freq1, reset, newQ[5:3], newQ[2:0], RGB1, RGB2, rowD, LAT, OE, donesending);
	
//	logic [5:0] RGBbus; 
//	logic [1:0] dummyclock;
// logic pls;
//	logic [8:0] memorylocation;
//	assign enable = (cnt==3'b110);
//	assign we = (cnt==3'b000);
////	always_ff @(posedge clk)
////		dummyclock <=dummyclock +1;
////	
////	assign pls = (donesending)? dummyclock[1] : 0;
////	
////	assign freq2 = pls;
////	
////	assign full = (memorylocation ==0); 
//		
//	always_ff @(negedge enable) //finished 
//		if (reset) memorylocation <= 0;
//		else  memorylocation <= memorylocation +1;
////		else if (!donesending) memorylocation <= memorylocation +1;
////		else  memorylocation <= 0;
//	
//	mem memory(freq1,we, freq1, memorylocation, newQ, RGBbus, donesending);		
//	matrix matrix(freq1, reset, RGBbus[5:3], RGBbus[2:0], RGB1, RGB2, rowD, LAT, OE, donesending);

	
	

			
endmodule



