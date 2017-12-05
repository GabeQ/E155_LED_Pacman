//microps project for memory 
//Created by Kai Kaneshina and Gabriel Quiroz
//code via David Money Harris

module mem(	input logic clk, we, fastclock,
				input logic [8:0] adr, 
				input logic [5:0] wd,
				output logic [5:0] rd,
				input logic donesending);
				
	logic [5:0] mem [511:0]; 
	logic [5:0] mem2 [511:0];
	logic [8:0] adr2;
	always@(negedge clk)
		begin  
				rd <= mem[adr];// should be uncommented for OG 
				 
			if (we) mem[adr] <= wd;
		end

//			
//	always_ff@(posedge fastclock)
//		 adr2 <= adr2 +1; //(adr==0) 
//		
//		
//
//	always_ff@(negedge fastclock)
//		  rd<=mem[adr2]; //(adr==0)
//
//		
endmodule 