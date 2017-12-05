//microps project to drive LED Matrix
//Created by Kai Kaneshina and Gabriel Quiroz
module matrix(input logic 			clk, reset,
				  input logic 	[2:0] RGB1bus,RGB2bus,
				  output logic [2:0] RGB1, RGB2, 
				  output logic [3:0] rowD,
				  output logic 		LAT,OE);
			
// RGB1[2:0]: R1=RGB1[2], G1=RGB1[1], B1=RBG1[0]
// RGB2[2:0]: R2=RGB2[2], G2=RGB2[1], B1=2=RBG2[0] 
//	rowD[3:0]: D = rowD[3], C=rowD[2], B=rowD[1], A=rowD[0]
	
	logic [4:0] counter;
	
	always_ff @ (negedge clk, posedge reset)
		if (reset) begin
			counter <= 0;
			RGB1 <= 0;
			RGB2 <= 0;
			rowD <= 0;
		end else begin
			counter <= counter + 1'b1;
			RGB1 <= RGB1bus;
			RGB2 <= RGB2bus;
			if (&counter)		rowD <= rowD + 1; //&
			else						rowD <= rowD;
		end
	
	always_ff @(negedge clk)
//		if (counter == 1) begin
//			LAT = 1;
//			OE = 0;
//			end
		if (counter==0) begin //&  is this value should be without memory, offset by 1 to cancel the delay, now value of 0
			LAT <= 1;
			OE <= 1;
		end else if (counter == 1) //0 is this value should be without memory, offset by 1 to cancel the delay, now value of 1
			LAT <= 0;
		else if(counter == 2 ) //1 is this value should be without memory, offset by 1 to cancel the delay, now value of 2
			OE <= 0;
		
endmodule 