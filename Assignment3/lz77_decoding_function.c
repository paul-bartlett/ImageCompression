#include "lz77_decoding_function.h"

void Decode_Using_LZ77(char *in_compressed_filename_Ptr) {

    char c;
    int row, col, width, height, maxGrayValue, searching_buffer_size, tokens;
    struct PGM_Image pgmImage;

    // Open file for reading
    FILE *lzFilePointer =  fopen(in_compressed_filename_Ptr, "rb");
    if(lzFilePointer == NULL) printf("Error opening lz file for reading");

    // Make sure the first char is P
    if(fgetc(lzFilePointer) != 'P') { 
        printf("Invalid PGM image: missing P");
        fclose(lzFilePointer);
    }

    // Make sure the second char is either a 2 or 5
    c = fgetc(lzFilePointer);
    if(c != '2' && c != '5') { 
        printf("Invalid PGM image: missing 2 or 5");
        fclose(lzFilePointer);
    }

    // Get the width, height, max gray value, and searching buffer size of the image
    width = geti(lzFilePointer); 
    height = geti(lzFilePointer);
    maxGrayValue = geti(lzFilePointer);
    searching_buffer_size = geti(lzFilePointer);
    tokens = geti(lzFilePointer);

    create_PGM_Image(&pgmImage, width, height, maxGrayValue);

    // Get all values from the offset, matching lengths, and next symbol arrays
    int *offset = calloc(tokens, sizeof(int)), 
        *matching_length = calloc(tokens, sizeof(int)), 
        *next_symbol = calloc(tokens, sizeof(int));

    for(int i = 0; i < tokens; i++) 
        offset[i] = geti(lzFilePointer);

    for(int i = 0; i < tokens; i++) 
        matching_length[i] = geti(lzFilePointer);

    for(int i = 0; i < tokens; i++) 
        next_symbol[i] = geti(lzFilePointer);

    fclose(lzFilePointer);
}
