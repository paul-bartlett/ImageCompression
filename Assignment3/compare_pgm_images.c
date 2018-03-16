#include "mean_absolute_error.h"

int main(int argc, char **argv) {

    if(argc != 3) {
        printf("You must supply 2 arguments: PGM file name 1, PGM file name 2\n");
        return 0;
    }

    char *pgmImage1 = argv[1],
         *pgmImage2 = argv[2];

    float mae = mean_absolute_error(pgmImage1, pgmImage2);
    
    printf("Mean absolute error: %.2f\n", mae);
}

