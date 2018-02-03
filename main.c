#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "libpnm.h"

#define PBM 1
#define PGM 2
#define PPM 3

int main(int argc, char *argv[])  { 

    int row, col, max_size;

    if(argc != 6) {
        printf("You must supply 5 arguments: image type code (1 for pbm, 2 for pgm, or 3 for ppm), image width, image height, output image name, and image format (0 for ASCII, 1 raw)\n");
        return -1;
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
            return -1;
        }
    } else if(image_type == PPM) {
        if(image_width >= 6 && image_width % 6 == 0 && image_height >= 4 && image_height % 4 == 0)
            printf("good size\n");
        else {
            printf("Width must be greater or equal to 6 and divisible by 6, and height must be greater or equal to 4 and divisible by 4\n");
            return -1;
        }
    } else {
        printf("Please enter a valid image type (1-3)\n");
        return -1;
    }   

    struct PBM_Image pic_pbm;
    create_PBM_Image(&pic_pbm, image_width, image_height); 

    // Iterate through all pixels and set pixel white if in the middle, else set to black
    for(row = 0; row<pic_pbm.height; row++) {
        for(col = 0; col<pic_pbm.width; col++) {
            if(row > pic_pbm.height/4 && row < (pic_pbm.height*3)/4 && col > pic_pbm.width/4 && col < (pic_pbm.width*3)/4)
                pic_pbm.image[row][col] = 0; // white
            else
                pic_pbm.image[row][col] = 1; // black
        }
    }

    float slope = (float)pic_pbm.height / (float)pic_pbm.width;
        printf("slope: %.5f\n", slope);
    if(pic_pbm.height>pic_pbm.width) {
        max_size = pic_pbm.height;
        for(row = 0; row<max_size; row++) {
            pic_pbm.image[row][(int)(row/slope)] = 1; // black
            pic_pbm.image[row][pic_pbm.width - 1 - (int)(row/slope)] = 1; 
        }
    } else {
        max_size = pic_pbm.width;
        for(col = 0; col<max_size; col++) {
            pic_pbm.image[(int)(col*slope)][col] = 1; // black
            pic_pbm.image[pic_pbm.height - 1 - (int)(col*slope)][col] = 1; 
        }
    }

    save_PBM_Image(&pic_pbm, "pic_1.pbm", true);
    free_PBM_Image(&pic_pbm);
}

