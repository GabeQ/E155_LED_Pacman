
//pacman toplevel module
//Created by Kai Kaneshina and Gabriel Quiroz
//2017

module e155_pacman(input logic 				clk, reset, 
						 input logic 	[3:0]		row,					//for the keypad
						 input logic				sck,					//sck: from master
					    input logic				mosi,					//mosi: from master
					    output logic				miso, freq1,		//miso: to master, freq1 is the clock going to the matrix
						 output logic  [2:0] 	RGB1, RGB2,			//color data going to the matrix
						 output logic 				LAT, OE,				//OE erases the board, LAT takes data from shift registers
						 output logic 	[3:0]		col, LEDS, rowD);	//col is for keypad. LEDS the keypad output. rowD chooses the matrix row.
						 
	logic [7:0] q;
	logic [2:0] cnt; 
		
	assign freq1 = &cnt; // clock frequency that the matrix runs on. based off when SPI finished sending signal
	keypad keypad(clk, reset, row, col, LEDS); //sends keypad data back to FPGA
	spi_pi_fpga spi_pi_fpga(sck, mosi, miso, reset, LEDS, q,cnt);	//SPI module

	logic [5:0] newQ;
	assign newQ = (q[6:4]<<3) | q[2:0]; //reduces the 8 bits of data into 2 3-bits of colors
	
	matrix matrix(freq1, reset, newQ[5:3], newQ[2:0], RGB1, RGB2, rowD, LAT, OE); //operates the matrix
			
endmodule



