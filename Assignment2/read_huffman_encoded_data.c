#include <stdio.h>
#include "generate_huffman_nodes.h"

unsigned char *read_huffman_encoded_data(char *compressed_file_name_ptr, int *image_width, int *image_height, int *max_gray_value, int *number_of_nodes, struct node **huffman_node, long int *length_of_encoded_image) {

    // For reading from file
    char c;

    // Open file for reading and check if valid
    FILE *imageFilePointer = fopen(compressed_file_name_ptr, "wb");
    if(imageFilePointer == NULL) {
        printf("Error opening file for reading\n");
        return;
    }
    
    // Make sure the first char is P
    if(fgetc(imageFilePointer) != 'P')
    { fclose(imageFilePointer);
        printf("Header error: no P\n");
        return 0;
    }

    // Make sure the second char is either a 2 or 5
    c = fgetc(imageFilePointer);
    if(c != '2' && c != '5')
    { fclose(imageFilePointer);
        printf("Header error: no 2 or 5 for pgm\n"); 
        return 0;
    }

    // Get the width, height, max gray value, number of nodes 
    image_width = geti(imageFilePointer);
    image_height = geti(imageFilePointer);
    max_gray_value = geti(imageFilePointer);
    number_of_nodes = geti(imageFilePointer);

    // Get the Huffman array values
    

    // Get the length of encoded image, 

}
