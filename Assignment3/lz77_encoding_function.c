#include "lz77_encoding_function.h"

void Encode_Using_LZ77(char *in_PGM_filename_Ptr, unsigned int searching_buffer_size,  float *avg_offset_Ptr, float *std_offset_Ptr, float *avg_length_Ptr, float *std_length_Ptr) {
    
    // Image variables
    struct PGM_Image pic_pgm;
    load_PGM_Image(&pic_pgm, in_PGM_filename_Ptr);
    int pixel_count = pic_pgm.height * pic_pgm.width; 
    
    // LZ77 token arrays
    int *offset = calloc(pixel_count, sizeof(int)), 
        *matching_length = calloc(pixel_count, sizeof(int)), 
        *next_symbol = calloc(pixel_count, sizeof(int));  

    int current_buffer_size = 0, token_size = 0;
    
    // Iterate through all pixels
    for(int row = 0; row < pic_pgm.height; row++) {
        for(int col = 0; col < pic_pgm.width; col++) {
            // Buffer through previous values and search for matches
            for(int i = 0; i < searching_buffer_size && i < current_buffer_size; i++) {
                if(current_buffer_size == searching_buffer_size)
            }
            current_buffer_size += 1;
            token_size += 1;
        }
    }
}
