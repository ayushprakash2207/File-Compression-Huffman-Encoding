#include"Huffman_Encode.h"

#define BUFFER_SIZE 1024

void calculate_histogram(uint8 u8ImgData[INPUT_SIZE], ap_uint<20> u8Histogram[INPUT_SYMBOL_SIZE])
{
  // Loop to process blocks of data
  for(ap_uint<20> i = 0; i < INPUT_SIZE; i++)
  {
//#pragma HLS UNROLL FACTOR = 4
#pragma HLS PIPELINE II = 2
	  uint8 u8Char = u8ImgData[i];
	  u8Histogram[u8Char]++;
  }
}
