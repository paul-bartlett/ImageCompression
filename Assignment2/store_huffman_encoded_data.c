#include <stdio.h>
#include "generate_huffman_nodes.h"

void store_huffman_encoded_data(char *compressed_file_name_ptr, int image_width, int image_height, int max_gray_value, int number_of_nodes, struct node *huffman_node, long int length_of_encoded_image, unsigned char *encoded_image) {

    // Open file for writing and check if valid
    FILE *imageFilePointer = fopen(compressed_file_name_ptr, "wb");
    if(imageFilePointer == NULL) {
        printf("Error opening file for writing\n");
        return;
    }

    // Write the header
    fprintf(imageFilePointer, "P2\n%d %d\n%d\n%d\n", image_width, image_height, max_gray_value, number_of_nodes);
    for(int i = 0; i < number_of_nodes; i++) {
        fprintf(imageFilePointer, "%d %d ", huffman_node[i].first_value, huffman_node[i].second_value);
    }
    fprintf(imageFilePointer, "\n%ld\n", length_of_encoded_image);
    
    // All values in encoded image
    fprintf(imageFilePointer, "%s\n", encoded_image);
}
