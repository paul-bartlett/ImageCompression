#include <stdio.h>
#include <stdlib.h>
#include "libpnm.h"
#include "generate_pixel_frequency.h"

long int *generate_pixel_frequency(struct PGM_Image *input_pgm_image, int *number_of_non_zero_values_in_the_frequency_array) {

    int max_gray_value = input_pgm_image->maxGrayValue;
    printf("%d %d", max_gray_value, *number_of_non_zero_values_in_the_frequency_array);
    //long int 
    return 0;
}

int main() {

    struct PGM_Image pic_pgm;
    create_PGM_Image(&pic_pgm, 256, 256, 256);
    int non_zero = 10;
    long int *test = generate_pixel_frequency(&pic_pgm, &non_zero);
    printf("%ld ", *test);
    return 0;
}

