#include <time.h>
#include "lz77_encoding_function.h"

int main(int argc, char **argv) {
    
    float offset_avg, offset_std, length_avg, length_std;
    double compression_time;

    if(argc != 3) {
        printf("You must supply 2 arguments: pgm image name, searching buffer size\n");
        return 0;
    }

    char *PGM_image = argv[1];
    unsigned int searching_buffer_size = atoi(argv[2]);
    
    clock_t begin = clock();
    Encode_Using_LZ77(PGM_image, searching_buffer_size, &offset_avg, &offset_std, &length_avg, &length_std);
    clock_t end = clock();
    compression_time = (double)(end - begin) / CLOCKS_PER_SEC;

    printf("Offset average: %.2f\nOffset standard deviation: %.2f\nMatch length average: %.2f\nMatch length standard deviation: %.2f\nCompression time: %.2f\n", offset_avg, offset_std, length_avg, length_std, compression_time);
}

