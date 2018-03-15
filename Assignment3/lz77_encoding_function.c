#include "lz77_encoding_function.h"

void Encode_Using_LZ77(char *in_PGM_filename_Ptr, unsigned int searching_buffer_size,  float *avg_offset_Ptr, float *std_offset_Ptr, float *avg_length_Ptr, float *std_length_Ptr) {

    // Image variables
    struct PGM_Image pic_pgm;
    load_PGM_Image(&pic_pgm, in_PGM_filename_Ptr);

    int *pixel_array = malloc(pic_pgm.height * pic_pgm.width * sizeof(int)), pixel_count = 0;

    // Iterate through all pixels to flatten array
    for(int row = 0; row < pic_pgm.height; row++) {
        for(int col = 0; col < pic_pgm.width; col++) {
            pixel_array[pixel_count] = pic_pgm.image[row][col];
            pixel_count++;
        }
    }
    
    // LZ77 token arrays
    int *offset = calloc(pixel_count, sizeof(int)), 
        *matching_length = calloc(pixel_count, sizeof(int)), 
        *next_symbol = calloc(pixel_count, sizeof(int));

    int current_buffer_size = 0, matches, pix;
    
    // Iterate through all pixels to create all LZ77 tokens
    for(pix = 0; current_buffer_size < pixel_count; pix++) {
        // Buffer through previous values and search for matches
        for(int i = 1; i <= searching_buffer_size && i <= current_buffer_size; i++) {
            matches = 0;
            // Continue looping for more matches
            while(pixel_array[current_buffer_size + matches] == pixel_array[current_buffer_size - i + matches] && current_buffer_size + matches < pixel_count) { // second might overflow
                matches++;
            }
            // Update match length only if larger
            if(matches > matching_length[pix]) {
                matching_length[pix] = matches;
                offset[pix] = i;
            }
        }
        // Include matched characters when determining next symbol, then add that one to buffer size
        current_buffer_size += matching_length[pix];
        next_symbol[pix] = pixel_array[current_buffer_size];
        current_buffer_size++;
    }

    // Create files and file names
    int buffer_string_size = (int)(ceil(log10(searching_buffer_size))+1);
    char buffer_string[buffer_string_size];
    sprintf(buffer_string, "%d", searching_buffer_size);
    char lzFileName[strlen(in_PGM_filename_Ptr) + buffer_string_size + 5];
    char offsetsFileName[strlen(in_PGM_filename_Ptr) + buffer_string_size + 14];
    char lengthsFileName[strlen(in_PGM_filename_Ptr) + buffer_string_size + 14];
    strcpy(lzFileName, in_PGM_filename_Ptr);
    strcpy(lzFileName, ".");
    strcpy(lzFileName, buffer_string);
    strcpy(lzFileName, ".lz");
    strcpy(offsetsFileName, in_PGM_filename_Ptr);
    strcpy(offsetsFileName, ".");
    strcpy(offsetsFileName, buffer_string);
    strcpy(offsetsFileName, ".offsets.csv");
    strcpy(lengthsFileName, in_PGM_filename_Ptr);
    strcpy(lengthsFileName, ".");
    strcpy(lengthsFileName, buffer_string);
    strcpy(lengthsFileName, ".lengths.csv");

    FILE *lzFilePointer = fopen(lzFileName, "wb");
    if(lzFilePointer == NULL) printf("Error opening lz file for writing");

    // Write the header
    fprintf(lzFilePointer, "P2\n%d %d\n%d\n", pic_pgm.width, pic_pgm.height, searching_buffer_size);

    // Write the LZ77 arrays for offsets, matching lengths, and next symbols
    for(int i = 0; i < pix; i++)
        fprintf(lzFilePointer, "%d ", offset[i]);
    fprintf(lzFilePointer, "\n");

    for(int i = 0; i < pix; i++)
        fprintf(lzFilePointer, "%d ", matching_length[i]);
    fprintf(lzFilePointer, "\n");

    for(int i = 0; i < pix; i++)
        fprintf(lzFilePointer, "%d ", next_symbol[i]);
    fprintf(lzFilePointer, "\n");

    fclose(lzFilePointer);

    // Calculate the average and standard deviation of the offsets and match lengths

    free(pixel_array);
    free(offset);
    free(matching_length);
    free(next_symbol);
}
