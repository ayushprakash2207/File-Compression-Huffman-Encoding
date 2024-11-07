#include "Huffman_Encode.h"
#include "assert.h"


void Huffman_Encode(uint8 u8ImgData[INPUT_SIZE], ap_uint<1> start, ap_uint<1> reset, ap_uint<1> *done, PackedCodewordAndLength encoding[INPUT_SYMBOL_SIZE])
{
	ap_uint<20> u8Histogram[INPUT_SYMBOL_SIZE] = {0};
    static uint8 u8Index = 0;
    static uint32 u32Index2 = 0;
    static uint8 u8Index3 = 0;
    static uint8 u8Counter = 0;
    static State currentState = RESET;
    static ap_uint<1> done_internal = 0;
	static uint8 num_symbols = 0;

    *done = done_internal;

    Symbol filter[INPUT_SYMBOL_SIZE];
    Symbol sort[INPUT_SYMBOL_SIZE];
    Symbol sort1[INPUT_SYMBOL_SIZE];
    Symbol sort2[INPUT_SYMBOL_SIZE];

    ap_uint<SYMBOL_BITS> parent[INPUT_SYMBOL_SIZE-1];
    ap_uint<SYMBOL_BITS> left[INPUT_SYMBOL_SIZE-1];
    ap_uint<SYMBOL_BITS> right[INPUT_SYMBOL_SIZE-1];

    ap_uint<SYMBOL_BITS> length_histogram[TREE_DEPTH];

    ap_uint<SYMBOL_BITS> output_length_histogram1[TREE_DEPTH];
    ap_uint<SYMBOL_BITS> output_length_histogram2[TREE_DEPTH];

    CodewordLength symbol_bits[INPUT_SYMBOL_SIZE];
    //PackedCodewordAndLength encoding[INPUT_SYMBOL_SIZE];

    int previous_frequency = -1;

    #pragma HLS INTERFACE ap_ctrl_none port=return
    #pragma HLS INTERFACE ap_memory port=u8ImgData
    #pragma HLS INTERFACE ap_none port=reset
    #pragma HLS INTERFACE ap_none port=start
    #pragma HLS INTERFACE ap_none port=done
    #pragma HLS INTERFACE ap_memory port=encoding

    switch (currentState) {
        case RESET:
            if (reset) {
                for (u8Index = 0; u8Index < 128; u8Index++) {
                    u8Histogram[u8Index] = 0;
                }
                u8Counter = 0;
                u32Index2 = 0; // Reset loop counter
                done_internal = 0;
            }
            currentState = CALCULATE_HISTOGRAM;
            break;

        case CALCULATE_HISTOGRAM:
            if (start)
            {
            	calculate_histogram(u8ImgData, u8Histogram);
                currentState = FILTERING;
            }
            break;

        case FILTERING:
        	filter_histogram(u8Histogram, filter, &num_symbols);
			currentState = SORT; // Transition to DONE after sorting
        	break;

        case SORT:
        	Sort(filter, num_symbols, sort);
        	currentState = CREATE_COPY;
        	break;

        case CREATE_COPY:
        	copy_sorted:
        	for(int i = 0; i < num_symbols ; i++) {
        	    sort1[i].value = sort[i].value;
        	    sort1[i].frequency = sort[i].frequency;
        	    sort2[i].value = sort[i].value;
        	    sort2[i].frequency = sort[i].frequency;
        	    assert(previous_frequency <= (int)sort[i].frequency);
        	    previous_frequency = sort[i].frequency;
        	}
        	currentState = CREATE_TREE;
        	break;

        case CREATE_TREE:
        	create_tree(sort1, num_symbols, parent, left, right);
        	currentState = CALC_BIT_LEN;
        	break;

        case CALC_BIT_LEN:
        	compute_bit_length(parent, left, right, num_symbols, length_histogram);
        	currentState = TRUNCATE_TREE;
        	break;

        case TRUNCATE_TREE:
        	truncate_tree(length_histogram, output_length_histogram1, output_length_histogram2);
        	currentState = CANONIZE_TREE;
        	break;

        case CANONIZE_TREE:
        	canonize_tree(sort2, num_symbols, output_length_histogram1, symbol_bits);
        	currentState = CREATE_CODE;
        	break;

        case CREATE_CODE:
        	create_codeword(symbol_bits, output_length_histogram2, encoding);
			currentState = DONE;
			break;

        case DONE:
			done_internal = 1;
			if (start) { // Transition to RESTARTING if reset is asserted
				currentState = RESTARTING;
			}
			break;

		case RESTARTING:
			if (start) {
				// Always reset the histogram in RESTARTING state
				for (u8Index = 0; u8Index < 128; u8Index++) {
					u8Histogram[u8Index] = 0;
				}
				u8Counter = 0; // Reset counter
				u32Index2 = 0; // Reset loop index
				currentState = CALCULATE_HISTOGRAM;  // Transition to RESET
			}
			break;
    }
}
