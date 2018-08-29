
module kernel (
	clk_clk,
	ir1498_0_conduit_end_0_export,
	lcd1602_demo_0_conduit_end_0_export_data,
	lcd1602_demo_0_conduit_end_0_export_rw,
	lcd1602_demo_0_conduit_end_0_export_en,
	lcd1602_demo_0_conduit_end_0_export_rs,
	lcd1602_demo_0_conduit_end_0_export_blon,
	lcd1602_demo_0_conduit_end_0_export_on,
	new_sdram_controller_0_wire_addr,
	new_sdram_controller_0_wire_ba,
	new_sdram_controller_0_wire_cas_n,
	new_sdram_controller_0_wire_cke,
	new_sdram_controller_0_wire_cs_n,
	new_sdram_controller_0_wire_dq,
	new_sdram_controller_0_wire_dqm,
	new_sdram_controller_0_wire_ras_n,
	new_sdram_controller_0_wire_we_n,
	reset_reset_n,
	seg7_demo_0_conduit_end_0_export);	

	input		clk_clk;
	input		ir1498_0_conduit_end_0_export;
	inout	[7:0]	lcd1602_demo_0_conduit_end_0_export_data;
	output		lcd1602_demo_0_conduit_end_0_export_rw;
	output		lcd1602_demo_0_conduit_end_0_export_en;
	output		lcd1602_demo_0_conduit_end_0_export_rs;
	output		lcd1602_demo_0_conduit_end_0_export_blon;
	output		lcd1602_demo_0_conduit_end_0_export_on;
	output	[12:0]	new_sdram_controller_0_wire_addr;
	output	[1:0]	new_sdram_controller_0_wire_ba;
	output		new_sdram_controller_0_wire_cas_n;
	output		new_sdram_controller_0_wire_cke;
	output		new_sdram_controller_0_wire_cs_n;
	inout	[31:0]	new_sdram_controller_0_wire_dq;
	output	[3:0]	new_sdram_controller_0_wire_dqm;
	output		new_sdram_controller_0_wire_ras_n;
	output		new_sdram_controller_0_wire_we_n;
	input		reset_reset_n;
	output	[63:0]	seg7_demo_0_conduit_end_0_export;
endmodule
