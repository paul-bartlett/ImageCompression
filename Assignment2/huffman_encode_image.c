#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libpnm.h"
#include "generate_huffman_nodes.h"

unsigned char *huffman_encode_image(struct PGM_Image *input_pgm_image, struct node *huffman_node, int number_of_nodes, unsigned long int *length_of_encoded_array) {
    
    int val1, val2, height = input_pgm_image->height, width = input_pgm_image->width, max_gray_value = input_pgm_image->maxGrayValue, gray_value;

    unsigned char *huffman_image = malloc(sizeof(unsigned char) * (height * width * number_of_nodes)); // Max size, maybe optimize with dynamic allocation
    huffman_image[0] = '\0';

    // Initialize array for each Huffman code with null terminator for strcpy
    char huffman_codes[max_gray_value][number_of_nodes];
    for(int i = 0; i < number_of_nodes; i++) 
        huffman_codes[number_of_nodes][0] = '\0';

    // Generate a Huffman code from the array of nodes in reverse order
    for(int i = number_of_nodes-1; i >= 0; i--) {
        val1 = huffman_node[i].first_value;
        val2 = huffman_node[i].second_value;
        strcpy(huffman_codes[val1], "0");
        strcpy(huffman_codes[val2], "1");
    }

    // Encode image into array 
    for(int row = 0; row < height; row++) {
        for(int col = 0; col < width; col++) {
            gray_value = input_pgm_image->image[row][col];
            strcpy(huffman_image, huffman_codes[gray_value]); // try another copy
            //printf("%s, ", huffman_image);
        }
    }

    return huffman_codes[0]; // not right
}
