module SEG7_driver(input csi_clk,input csi_rst_n,input avs_cs,input [4:0]avs_add,input avs_rd,input avs_wr,input [31:0]avs_wrd,output reg [31:0]avs_rdd,output reg [63:0]coe_SEG7);
	always@(posedge csi_clk or negedge csi_rst_n) begin
		if (!csi_rst_n) begin
			coe_SEG7<=64'hffffffffffffffff;
		end
		else if ((avs_cs)&&(avs_wr)&&(avs_add==5'b00000)) begin
				if (avs_wrd!=32'hffffffff) begin
					coe_SEG7<={4'h0,avs_wrd[31:28],4'h0,avs_wrd[31:28],4'h0,avs_wrd[31:28],4'h0,avs_wrd[27:24],4'h0,avs_wrd[23:20],4'h0,avs_wrd[19:16],4'h0,avs_wrd[15:12],4'h0,avs_wrd[11:8],4'h0,avs_wrd[7:4],4'h0,avs_wrd[3:0]};
				end
				else begin
					coe_SEG7<=64'hffffffffffffffff;
				end
		end
	end
endmodule
