#include <stdio.h>
#include <stdlib.h>
#include "libpnm.h"

#define PBM 1
#define PGM 2
#define PPM 3

int main(int argc, char *argv[])  {

    if(argc != 6) {
        printf("You must supply 5 arguments: image type code (1 for pbm, 2 for pgm, or 3 for ppm), image width, image height, output image name, and image format (0 for ASCII, 1 raw)\n");
        exit(1);
    }
    
    int image_type = atoi(argv[1]);
    int image_width = atoi(argv[2]);
    int image_height = atoi(argv[3]);
    char * output_image = argv[4];
    int image_format = atoi(argv[5]);
    printf("%d %d %d %s %d\n", image_type, image_width, image_height, output_image, image_format);
    
    if(image_type == PBM || image_type == PGM) {
        if(image_width >= 4 && image_width % 4 == 0 && image_height >= 4 && image_height % 4 == 0)
            printf("good size\n");
        else {
            printf("Width must be greater or equal to 4 and divisible by 4, and height must be greater or equal to 4 and divisible by 4\n");
            exit(1);
        }
    } else if(image_type == PPM) {
        if(image_width >= 6 && image_width % 6 == 0 && image_height >= 4 && image_height % 4 == 0)
            printf("good size\n");
        else 
            printf("Width must be greater or equal to 6 and divisible by 6, and height must be greater or equal to 4 and divisible by 4\n");
        exit(1);
    } else {
        printf("Please enter a valid image type (1-3)\n");
        exit(1);
    }   
}
