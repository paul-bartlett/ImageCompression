#include "lz77_decoding_function.h"

void Decode_Using_LZ77(char *in_compressed_filename_Ptr) {

    char c;
    int row, col, width, height, maxGrayValue, searching_buffer_size, current_buffer_size = 0, pixel_count, tokens;
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
    pixel_count = width * height;
    int *offset = calloc(tokens, sizeof(int)), 
        *matching_length = calloc(tokens, sizeof(int)), 
        *next_symbol = calloc(tokens, sizeof(int)),
        *pixel_array = malloc(pixel_count * sizeof(int));


    for(int i = 0; i < tokens; i++) 
        offset[i] = geti(lzFilePointer);

    for(int i = 0; i < tokens; i++) 
        matching_length[i] = geti(lzFilePointer);

    for(int i = 0; i < tokens; i++) 
        next_symbol[i] = geti(lzFilePointer);

    fclose(lzFilePointer);

    // Decode the data from the arrays
    for(int tok = 0; tok < tokens; tok++) {
        if(matching_length[tok] > searching_buffer_size)
            printf("Matching length greater than buffer\n");
        // Add matching length number of items 
        for(int i = 0; i < matching_length[tok]; i++) {
            pixel_array[current_buffer_size] = pixel_array[current_buffer_size - offset[tok]];
            current_buffer_size++;
        }
        pixel_array[current_buffer_size] = next_symbol[tok];
        current_buffer_size++;
    }
	
    pixel_count = 0;
    // Fill image with decoded values
    for(row = 0; row < height; row++) {
        for(col = 0; col < width; col++) {
            pgmImage.image[row][col] = pixel_array[pixel_count];
            pixel_count++;
        }
    }

    // Save image
    char lzFileName[strlen(in_compressed_filename_Ptr) + 5];
    sprintf(lzFileName, "%s.pgm", in_compressed_filename_Ptr);
    save_PGM_Image(&pgmImage, lzFileName, 0);

    // Free memory
    free_PGM_Image(&pgmImage);
    free(offset);
    free(matching_length);
    free(next_symbol);
    free(pixel_array);
}

