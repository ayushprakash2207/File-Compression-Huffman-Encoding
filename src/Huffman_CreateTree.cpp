#include "Huffman_Encode.h"
#include "assert.h"

void create_tree(Symbol in[INPUT_SYMBOL_SIZE],
		         uint8 num_symbols,
				 ap_uint<SYMBOL_BITS> parent[INPUT_SYMBOL_SIZE - 1],
				 ap_uint<SYMBOL_BITS> left[INPUT_SYMBOL_SIZE - 1],
				 ap_uint<SYMBOL_BITS> right[INPUT_SYMBOL_SIZE - 1])
{
	Frequency frequency[INPUT_SYMBOL_SIZE - 1];
	ap_uint<SYMBOL_BITS> tree_count = 0;
	ap_uint<SYMBOL_BITS> in_count = 0;

	assert(num_symbols > 0);
	assert(num_symbols <= INPUT_SYMBOL_SIZE);
	for(int i = 0; i < (num_symbols - 1); i++)
	{
#pragma HLS PIPELINE II=5
		Frequency node_freq = 0;
		assert(in_count < num_symbols || tree_count < i);

		Frequency intermediate_freq = frequency[tree_count];
		Symbol s = in[in_count];
		 if((in_count < num_symbols && s.frequency <= intermediate_freq) || tree_count == i)
		 {
			 left[i] = s.value; // Set input symbol as left node
			 node_freq = s.frequency; // Add symbol frequency to total node frequency
			 in_count++; // Move to the next input symbol
		 }
		 else
		 {
			 // Pick internal node without a parent.
			  left[i] = INTERNAL_NODE; // Set symbol to indicate an internal node
			  node_freq = frequency[tree_count]; // Add child node frequency
			  parent[tree_count] = i; // Set this node as childs parent
			  tree_count++; // Go to next parentless internal node
		 }

		 assert(in_count < num_symbols || tree_count < i);
		 intermediate_freq = frequency[tree_count];
		 s = in[in_count];

		 if((in_count < num_symbols && s.frequency <= intermediate_freq) || tree_count == i)
		 {
			 // Pick symbol from in[].
			  right[i] = s.value;
			  frequency[i] = node_freq + s.frequency;
			  in_count++;
		 }
		 else
		 {
			 // Pick internal node without a parent.
			  right[i] = INTERNAL_NODE;
			  frequency[i] = node_freq + intermediate_freq;
			  parent[tree_count] = i;
			  tree_count++;
		 }
		 // Verify that nodes in the tree are sorted by frequency
		  assert(i == 0 || frequency[i] >= frequency[i-1]);
	}

	parent[tree_count] = 0; //Set parent of last node (root) to 0
}
