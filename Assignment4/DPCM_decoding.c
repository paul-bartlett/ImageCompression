#include <time.h>
#include "DPCM_decoding_function.h"

int main(int argc, char **argv) {

    double compression_time;

    if(argc != 2) {
        printf("You must supply 1 argument: a DPCM compressed file name\n");
        return 0;
    }

    char *DPCM_image = argv[1];

    clock_t begin = clock();
    Decode_Using_DPCM(DPCM_image);
    clock_t end = clock();
    compression_time = (double)(end - begin) / CLOCKS_PER_SEC;

    printf("Decompression time: %.2f\n", compression_time);
}

