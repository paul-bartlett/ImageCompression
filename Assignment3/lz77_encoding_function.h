#ifndef LZ77_ENCODING_FUNCTION_H
#define LZ77_ENCODING_FUNCTION_H

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "libpnm.h"

void Encode_Using_LZ77(char *in_PGM_filename_Ptr, unsigned int searching_buffer_size,  float *avg_offset_Ptr, float *std_offset_Ptr, float *avg_length_Ptr, float *std_length_Ptr);

#endif // LZ77_ENCODING_FUNCTION_H
