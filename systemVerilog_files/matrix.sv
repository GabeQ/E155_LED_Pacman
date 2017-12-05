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
		end else begin //on each clock cycle we increase the column, and shift in the corresponding colors
			counter <= counter + 1'b1; 
			RGB1 <= RGB1bus;
			RGB2 <= RGB2bus;
			if (&counter)		rowD <= rowD + 1'b1; //if the counter overflows, we know that the columns have been completed and thus we change the row to update
			else						rowD <= rowD;
		end

	always_ff @(negedge clk)
		if (&counter) begin //at the end of the 32nd cycle, we clear the board and then send all the shift register data
			LAT <= 1;
			OE <= 1;
		end else if (counter ==0 ) //at the end of the 33rd cycle, we then stop sending in the data
			LAT <= 0;
		else if(counter == 1) //deassert the blanking signal on the 34th cycle to allow the data to show on the matrix
			OE <= 0; 
		
endmodule 