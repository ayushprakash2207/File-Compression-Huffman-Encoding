#include <stdio.h>
#include <stdint.h>

#include "Huffman_Encode.h"

int main()
{
    // Declare and initialize input data (example input: characters with ASCII values from 0 to 127)
    uint8 input[128];
    uint8 output[128];
    ap_uint<1> done;
    ap_uint<1> reset;

    // Initialize the input array with some values (e.g., repeating a few characters)
    for (int i = 0; i < 128; i++) {
        input[i] = (i % 5) + 65;  // Example: repeating 'A', 'B', 'C', 'D', 'E'
    }

    // Reset phase: Reset the output array and internal states
    reset = 1;  // Reset signal
    Huffman_Encode(input, reset, &done, output);

    // Run phase: Process the input data
    reset = 0;  // Clear reset
    Huffman_Encode(input, reset, &done, output);

    // Check if the encoding is done
    if (done) {
        // Print the frequency table
        printf("Character Frequencies:\n");
        for (int i = 0; i < 128; i++) {
            if (output[i] > 0) {  // Print only characters that appear
                printf("Char %c (ASCII %d): %d times\n", i, i, output[i]);
            }
        }
    } else {
        printf("Huffman encoding did not complete.\n");
    }

    return 0;
}
