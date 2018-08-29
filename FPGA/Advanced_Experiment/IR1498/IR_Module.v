module IR_Module(
				//--------------Avalon-------------//
										csi_CLK,
										csi_RST_N,
										avs_chipselect,
										avs_address,
										avs_read,
										avs_readdata,
										avs_write,
										avs_writedata,
										ins_irq,																	
										
				//--------------IR Interface-------------//
										coe_iIRDA
										);
										
//	Avalon Side
input			           csi_CLK ;      //100Mhz
input              csi_RST_N ;
input		       	    avs_chipselect;
input [4:0]				    avs_address;
input						        avs_read;
output reg [31:0]		avs_readdata;
input						        avs_write;
input [31:0]			    avs_writedata;
output  reg       ins_irq;

//	IR Side
input  	           coe_iIRDA;


reg       Clk_50M;
reg [1:0]    DATA_READY_Pre;
wire [31:0]  mData;
wire         mDATA_READY;


//Div Clk  Input-Clk: 100MHz   Output-Clk:50MHz
//IR_RECEIVE 模块使用50MHz时钟
always@(posedge csi_CLK or negedge csi_RST_N)
 begin
  if(!csi_RST_N)
   begin
				Clk_50M<=1'b0;
			end 
  else  
   begin
	    Clk_50M<=~Clk_50M;
			end

 end

	
	// 1 Avalon Read Process
always @(posedge csi_CLK or negedge csi_RST_N) 
 begin		
		if(!csi_RST_N) begin
				ins_irq<=1'b0; //Clear Irq-Signal
				DATA_READY_Pre<=2'b00;
		end
		else	
		 begin
			 DATA_READY_Pre[0]<=mDATA_READY;
				DATA_READY_Pre[1]<=DATA_READY_Pre[0];
				if(DATA_READY_Pre[1:0]==2'b01)               // The rising edge is detected and the interrupt signal is valid .
				  ins_irq<=1'b1;    //SetIrq-Signal 
						
				if((avs_chipselect == 1) &&(avs_read == 1)) 
				 begin		 
		    	if(avs_address[4:0]==5'b00000) begin
								ins_irq<=1'b0; //Clear Irq-Signal
							 	avs_readdata<=mData;
								end
				end
		end
end	



IR_RECEIVE  u0(
					.iCLK(Clk_50M),              //clk 50MHz
					.iRST_n(csi_RST_N),          //reset					
					.iIRDA(coe_iIRDA),           //IR code input
					.oDATA_READY(mDATA_READY),   //data ready
					.oDATA(mData)                //decode data output
					);


endmodule 
