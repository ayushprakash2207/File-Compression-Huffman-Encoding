#include "Huffman_Encode.h"

void filter_histogram(ap_uint<20> in[128],
			Symbol out[128],
			uint8 *num_symbols)
{
#pragma HLS INLINE OFF
	ap_uint<SYMBOL_BITS> j = 0;
	for (int i = 0; i < 128; i++)
	{
#pragma HLS PIPELINE II=1
		if(in[i] != 0)
		{
			out[j].value = i;
			out[j].frequency = in[i];
			j++;
		}
	}
	*num_symbols = j;
}
