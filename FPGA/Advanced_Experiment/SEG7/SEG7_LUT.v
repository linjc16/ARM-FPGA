module SEG7_LUT	(	oSEG,iDIG	);
input	[7:0]	iDIG;
output	[6:0]	oSEG;
reg		[6:0]	oSEG;

always @(iDIG)
begin
		case(iDIG)
		8'h01: oSEG = 7'b1111001;	
		8'h02: oSEG = 7'b0100100; 	
		8'h03: oSEG = 7'b0110000; 	
		8'h04: oSEG = 7'b0011001; 	
		8'h05: oSEG = 7'b0010010; 	
		8'h06: oSEG = 7'b0000010; 	
		8'h07: oSEG = 7'b1111000; 	
		8'h08: oSEG = 7'b0000000; 	
		8'h09: oSEG = 7'b0010000; 	
		8'h0a: oSEG = 7'b0001000;
		8'h0b: oSEG = 7'b0000011;
		8'h0c: oSEG = 7'b1000110;
		8'h0d: oSEG = 7'b0100001;
		8'h0e: oSEG = 7'b0000110;
		8'h0f: oSEG = 7'b0001110;
		8'h00: oSEG = 7'b1000000;
		default: oSEG = 7'b1111111;
		endcase
end

endmodule

//
// ---0---
// |     |
// 5     1
// |     |
// ---6---
// |     |
// 4     2
// |     |
// ---3---