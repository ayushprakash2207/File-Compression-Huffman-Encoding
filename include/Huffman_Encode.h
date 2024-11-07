#ifndef __HUFFMAN_ENCODE_H__
#define __HUFFMAN_ENCODE_H__

#include <stdio.h>
#include <stdint.h>
#include <ap_int.h>

typedef uint8_t uint8;
typedef ap_uint<16> uint16;
typedef ap_uint<32> uint32;
typedef ap_uint<32> Frequency;

enum State {
    RESET,
    CALCULATE_HISTOGRAM,
	FILTERING,
	SORT,
	CREATE_COPY,
	CREATE_TREE,
	CALC_BIT_LEN,
	TRUNCATE_TREE,
	CANONIZE_TREE,
	CREATE_CODE,
    DONE,
	RESTARTING
};

#define INPUT_SIZE 102400

// input number of symbols
const static int INPUT_SYMBOL_SIZE = 256;

// Should be log2(INPUT_SYMBOL_SIZE)
const static int SYMBOL_BITS = 8;

// A marker for internal nodes
const static ap_uint<SYMBOL_BITS> INTERNAL_NODE = -1;

// upper bound on codeword length during tree construction
const static int TREE_DEPTH = 64;

// Should be log2(TREE DEPTH)
const static int TREE_DEPTH_BITS = 6;

const static int MAX_CODEWORD_LENGTH = 27;

//number of bits needed to record MAX_CODEWORD_LENGTH value
//Should be log2(MAX_CODEWORD_LENGTH)
const static int CODEWORD_LENGTH_BITS = 5;



typedef ap_uint<CODEWORD_LENGTH_BITS> CodewordLength;


typedef ap_uint<MAX_CODEWORD_LENGTH> Codeword;
typedef ap_uint<MAX_CODEWORD_LENGTH + CODEWORD_LENGTH_BITS> PackedCodewordAndLength;
typedef ap_uint<CODEWORD_LENGTH_BITS> CodewordLength;

struct Symbol
{
ap_uint<SYMBOL_BITS> value;
ap_uint<20> frequency;
};

void Huffman_Encode(uint8 u8ImgData[INPUT_SIZE], ap_uint<1> start, ap_uint<1> reset, ap_uint<1> *done, CodewordLength symbol_bits[INPUT_SYMBOL_SIZE]);

void calculate_histogram(uint8 u8ImgData[INPUT_SIZE], ap_uint<20> u8Histogram[INPUT_SYMBOL_SIZE]);

void filter_histogram(ap_uint<20> in[INPUT_SYMBOL_SIZE],
			Symbol out[INPUT_SYMBOL_SIZE],
			uint8 *num_symbols);

void Sort(Symbol in[INPUT_SYMBOL_SIZE], uint8 num_symbols ,Symbol out[INPUT_SYMBOL_SIZE]);

void create_tree(Symbol in[INPUT_SYMBOL_SIZE],
		         uint8 num_symbols,
				 ap_uint<SYMBOL_BITS> parent[INPUT_SYMBOL_SIZE - 1],
				 ap_uint<SYMBOL_BITS> left[INPUT_SYMBOL_SIZE - 1],
				 ap_uint<SYMBOL_BITS> right[INPUT_SYMBOL_SIZE - 1]);

void compute_bit_length(ap_uint<SYMBOL_BITS> parent[INPUT_SYMBOL_SIZE - 1],
				 	    ap_uint<SYMBOL_BITS> left[INPUT_SYMBOL_SIZE - 1],
				        ap_uint<SYMBOL_BITS> right[INPUT_SYMBOL_SIZE - 1],
						uint8 num_symbols,
						ap_uint<SYMBOL_BITS> length_histogram[TREE_DEPTH]);


void truncate_tree(ap_uint<SYMBOL_BITS> input_length_histogram[TREE_DEPTH],
				   ap_uint<SYMBOL_BITS> output_length_histogram1[TREE_DEPTH],
				   ap_uint<SYMBOL_BITS> output_length_histogram2[TREE_DEPTH]);

void canonize_tree(Symbol sorted[INPUT_SYMBOL_SIZE],
				   ap_uint<SYMBOL_BITS> num_symbols,
				   ap_uint<SYMBOL_BITS> codeword_length_histogram[TREE_DEPTH],
				   CodewordLength symbol_bits[INPUT_SYMBOL_SIZE]);

void create_codeword(
    /* input */ CodewordLength symbol_bits[INPUT_SYMBOL_SIZE],
    /* input */ ap_uint<SYMBOL_BITS> codeword_length_histogram[TREE_DEPTH],
    /* output */ PackedCodewordAndLength encoding[INPUT_SYMBOL_SIZE]
);

#endif //__HUFFMAN_ENCODE_H__
