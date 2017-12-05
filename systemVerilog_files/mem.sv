//microps project for memory 
//Created by Kai Kaneshina and Gabriel Quiroz
//code via David Money Harris

module mem(	input logic clk, we, 
				input logic [9:0] adr,
				input logic [2:0] wd,
				output logic [2:0] rd);
				
	logic [2:0] mem [1023:0];
	
	always @(negedge clk)
		begin 
			rd <= mem[adr];
			if (we) mem[adr] <= wd;
		end

		
	
endmodule 