#include <time.h>
#include "DPCM_encoding_function.h"

int main(int argc, char **argv) {
    
    float error_avg, error_std;
    double compression_time;
    
    if(argc != 3) {
        printf("You must supply 2 arguments: pgm image name, prediction rule code\n");
        return 0;
    }

    char *PGM_image = argv[1];
    unsigned int prediction_rule_code = atoi(argv[2]);
    
    clock_t begin = clock();
    Encode_Using_DPCM(PGM_image, prediction_rule_code, &error_avg, &error_std);
    clock_t end = clock();
    compression_time = (double)(end - begin) / CLOCKS_PER_SEC;

    printf("Absolute prediction error average: %.2f\nAbsolute prediction standard deviation: %.2f\nCompression time: %.2f\n", error_avg, error_std, compression_time);
}

