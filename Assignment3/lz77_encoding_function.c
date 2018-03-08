#include "lz77_encoding_function.h"

void Encode_Using_LZ77(char *in_PGM_filename_Ptr, unsigned int searching_buffer_size,  float *avg_offset_Ptr, float *std_offset_Ptr, float *avg_length_Ptr, float *std_length_Ptr) {
    
    int offset, matching_length, next_symbol, current_buffer_size = 0;
    struct PGM_Image pic_pgm;
    load_PGM_Image(&pic_pgm, in_PGM_filename_Ptr);

    for(int row = 0; row < pic_pgm.height; row++) {
        for(int col = 0; col < pic_pgm.width; col++) {

            for(int i = 0; i < searching_buffer_size && i < current_buffer_size; i++) {
                
            }
    }
}
