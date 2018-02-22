#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libpnm.h"
#include "generate_pixel_frequency.h"

long int *generate_pixel_frequency(struct PGM_Image *input_pgm_image, int *number_of_non_zero_values_in_the_frequency_array) {

    int max_gray_value = input_pgm_image->maxGrayValue;
    int height = input_pgm_image->height;
    int width = input_pgm_image->width;
    int non_zero = 0, gray_value;

    // Dynamic allocation of array with all values 0
    long int *frequency_array = calloc(max_gray_value + 1, sizeof(long int));
    if (!frequency_array)
        perror("malloc() failed");

    // Read all pixels to record frequency and count non-zero values
    for(int row = 0; row < height; row++) {
        for(int col = 0; col < width; col++) {
            gray_value = input_pgm_image->image[row][col];
            // Check if first occurence of gray value
            if((frequency_array[gray_value] += 1) == 1) {
                non_zero += 1;
            }
            //printf("%ld, ", frequency_array[gray_value]);
        }
    }
    printf("%d\n", non_zero);
    *number_of_non_zero_values_in_the_frequency_array = non_zero;
    return frequency_array;
}

//int main() {
//
//    struct PGM_Image pic_pgm;
//    create_PGM_Image(&pic_pgm, 256, 256, 256);
//    int non_zero = 10;
//    long int test = generate_pixel_frequency(&pic_pgm, &non_zero);
//    printf("%ld ", test);
//    return 0;
//}

