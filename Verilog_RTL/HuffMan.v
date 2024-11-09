`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 10.10.2024 12:46:25
// Design Name: 
// Module Name: HuffMan
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module HuffMan(
    input clk,
    input reset,
    input uart_rx_pin,
    output done
    );
    
wire start;
    
reg data_en1;
reg data_we1;
reg [16:0] data_addr1;
reg [7:0] data_din1;
wire [7:0] data_dout1;

wire data_en2;
reg data_we2;
wire [16:0] data_addr2;
reg [7:0] data_din2;
wire [7:0] data_dout2;

wire Encode_en1;
wire Encode_we1;
reg Encode_en2;
wire Encode_we2;
wire [7:0] Encode_addr1;
reg [7:0] Encode_addr2;
wire [31:0] Encode_din1;
wire [31:0] Encode_dout2;

wire [7:0] uart_rx_data;
wire uart_rx_valid;

reg rx_done;

reg [7:0] uart_tx_data;

// Parameters
parameter MEM_DEPTH = 102400;
parameter ASCII_SIZE = 256;

ila_0 ILA (
	.clk(clk), // input wire clk
	.probe0(Encode_en1), // input wire [0:0]  probe0  
	.probe1(Encode_we1), // input wire [0:0]  probe1 
	.probe2(Encode_addr1), // input wire [7:0]  probe2 
	.probe3(Encode_din1), // input wire [31:0]  probe3 
	.probe4(rx_done), // input wire [0:0]  probe4 
	.probe5(start), // input wire [0:0]  probe5 
	.probe6(done), // input wire [0:0]  probe6 
	.probe7(data_addr1) // input wire [16:0]  probe7
);

initial
begin
data_en1 <= 1;
data_we1 <= 1;
data_addr1 <= 0;
rx_done <= 0;
Encode_addr2 <= 0;
Encode_en2 <= 0;
end

top_controller_rx(clk, uart_rx_pin, reset, uart_rx_data, uart_rx_valid);

always @(posedge uart_rx_valid)
begin
    if(data_addr1 < MEM_DEPTH)
        data_addr1 <= data_addr1 + 1;
    if(data_addr1 == MEM_DEPTH - 1)
    begin
        rx_done <= 1;
        data_addr1 <= 0;
    end
end

assign start = (rx_done)? 1 :   0;
    
//always @(posedge clk)
//begin
//if(done)
//begin
//Encode_en2 <= 1;
//uart_tx_data <= data_dout2[7:0];
//end
//end

//top_controller_tx(clk, reset, Encode_en2, uart_tx_data, uart_tx_pin);
    
Img_Data Data_RAM (
  .clka(uart_rx_valid),    // input wire clka
  .ena(data_en1),      // input wire ena
  .wea(data_we1),      // input wire [0 : 0] wea
  .addra(data_addr1),  // input wire [6 : 0] addra
  .dina(uart_rx_data),    // input wire [7 : 0] dina
  .clkb(clk),    // input wire clkb
  .enb(data_en2),      // input wire enb
  .addrb(data_addr2),  // input wire [6 : 0] addrb
  .doutb(data_dout2)  // output wire [7 : 0] doutb
);

Encode Encode_RAM (
  .clka(clk),    // input wire clka
  .ena(Encode_en1),      // input wire ena
  .wea(Encode_we1),      // input wire [0 : 0] wea
  .addra(Encode_addr1),  // input wire [7 : 0] addra
  .dina(Encode_din1),    // input wire [31 : 0] dina
  .clkb(clk),    // input wire clkb
  .enb(Encode_en2),      // input wire enb
  .addrb(Encode_addr2),  // input wire [7 : 0] addrb
  .doutb(Encode_dout2)  // output wire [31 : 0] doutb
);

Huffman_Encode_0 Huffman_Core (
  .u8Data_ce0(data_en2),            // output wire u8ImgData_ce0
  .encoding_ce0(Encode_en1),              // output wire encoding_ce0
  .encoding_we0(Encode_we1),              // output wire encoding_we0
  .ap_clk(clk),                          // input wire ap_clk
  .ap_rst(reset),                          // input wire ap_rst
  .u8Data_address0(data_addr2),  // output wire [6 : 0] u8ImgData_address0
  .u8Data_q0(data_dout2),              // input wire [7 : 0] u8ImgData_q0
  .start_r(start),                        // input wire [0 : 0] start_r
  .reset(reset),                            // input wire [0 : 0] reset
  .done(done),                              // output wire [0 : 0] done
  .encoding_address0(Encode_addr1),    // output wire [7 : 0] encoding_address0
  .encoding_d0(Encode_din1)                // output wire [31 : 0] encoding_d0
);

endmodule
