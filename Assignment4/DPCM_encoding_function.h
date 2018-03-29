#ifndef DPCM_ENCODING_FUNCTION_H
#define DPCM_ENCODING_FUNCTION_H

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "libpnm.h"

void Encode_Using_DPCM(char *in_PGM_filename_Ptr, int prediction_rule, float *avg_abs_error_Ptr, float *std_abs_error_Ptr);

#endif // DPCM_ENCODING_FUNCTION_H  
