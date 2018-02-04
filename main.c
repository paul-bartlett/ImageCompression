#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "libpnm.h"

#define PBM 1
#define PGM 2
#define PPM 3

int main(int argc, char *argv[])  { 

    int row, col, col_count, row_count, gray_level, maximum_gray_level = 255;
    float slope, proportion;
    struct PBM_Image pic_pbm;
    struct PGM_Image pic_pgm;
    struct PPM_Image pic_ppm;

    if(argc != 6) {
        printf("You must supply 5 arguments: image type code (1 for pbm, 2 for pgm, or 3 for ppm), image width, image height, output image name, and image format (0 for ASCII, 1 raw)\n");
        return -1;
    }

    int image_type = atoi(argv[1]);
    int image_width = atoi(argv[2]);
    int image_height = atoi(argv[3]);
    char * output_image = argv[4];
    int image_format = atoi(argv[5]);
    printf("type: %d, width: %d, height: %d, name: %s, format: %d\n", image_type, image_width, image_height, output_image, image_format);

    // Check image size depending on image type
    if(image_type == PBM || image_type == PGM) {
        if(image_width >= 4 && image_width % 4 == 0 && image_height >= 4 && image_height % 4 == 0)
            printf("Good image size\n");
        else {
            printf("Width must be greater or equal to 4 and divisible by 4, and height must be greater or equal to 4 and divisible by 4\n");
            return -1;
        }
    } else if(image_type == PPM) {
        if(image_width >= 6 && image_width % 6 == 0 && image_height >= 4 && image_height % 4 == 0)
            printf("Good image size\n");
        else {
            printf("Width must be greater or equal to 6 and divisible by 6, and height must be greater or equal to 4 and divisible by 4\n");
            return -1;
        }
    } else {
        printf("Please enter a valid image type (1-3)\n");
        return -1;
    }   

    // Decide which of the 3 image types to create (PBM, PGM, PPM)
    switch(image_type) {

        case PBM:
            // PBM image creation
            create_PBM_Image(&pic_pbm, image_width, image_height); 

            // Iterate through all pixels and set pixel white if in the middle, else set to black
            for(row = 0; row<pic_pbm.height; row++) {
                for(col = 0; col<pic_pbm.width; col++) {
                    if(row >= pic_pbm.height/4 && row < (pic_pbm.height*3)/4 && col >= pic_pbm.width/4 && col < (pic_pbm.width*3)/4)
                        pic_pbm.image[row][col] = 0; // white
                    else
                        pic_pbm.image[row][col] = 1; // black
                }
            }
            printf("height 1/4: %d, height 3/4: %d, width 1/4: %d, width 3/4: %d\n", pic_pbm.height/4, (pic_pbm.height*3)/4, pic_pbm.width/4, (pic_pbm.width*3)/4);

            // Calculate slope for the corner to corner black pixels
            slope = (float)pic_pbm.height / (float)pic_pbm.width;
            printf("slope: %.5f\n", slope);

            // When height is greater, go row by row to cover every pixel in the line
            if(pic_pbm.height>pic_pbm.width) {
                for(row = 0; row<pic_pbm.height; row++) {
                    pic_pbm.image[row][(int)(row/slope)] = 1; // black
                    pic_pbm.image[row][pic_pbm.width - 1 - (int)(row/slope)] = 1; 
                }
                // Column by column to cover cases when width is greater (or square images)
            } else { 
                for(col = 0; col<pic_pbm.width; col++) {
                    pic_pbm.image[(int)(col*slope)][col] = 1; // black
                    pic_pbm.image[pic_pbm.height - 1 - (int)(col*slope)][col] = 1; 
                }
            }

            // Save image after setting pixels then cleanup 
            save_PBM_Image(&pic_pbm, output_image, image_format);
            free_PBM_Image(&pic_pbm);
            break;

        case PGM:
            // PGM image creation
            create_PGM_Image(&pic_pgm, image_width, image_height, maximum_gray_level); 

            // Iterate through all pixels and set pixel white if in the middle, else set to black
            for(row = 0; row<pic_pgm.height; row++) {
                for(col = 0; col<pic_pgm.width; col++) {
                    if(row >= pic_pgm.height/4 && row < (pic_pgm.height*3)/4 && col >= pic_pgm.width/4 && col < (pic_pgm.width*3)/4)
                        pic_pgm.image[row][col] = 255; // white
                    else
                        pic_pgm.image[row][col] = 0; // black
                }
            }
            printf("height 1/4: %d, height 3/4: %d, width 1/4: %d, width 3/4: %d\n", pic_pgm.height/4, (pic_pgm.height*3)/4, pic_pgm.width/4, (pic_pgm.width*3)/4);

            // Calculate slope for the increase in darkness towards the bottom
            slope = (float)pic_pgm.height / (float)pic_pgm.width;
            printf("slope: %.5f\n", slope);

            // Creates middle of image that gets increasingly dark towards the center
            row_count = 0; // initialize
            for(row = pic_pgm.height/4; row<pic_pgm.height/2; row++) {            
                col_count = 0;
                row_count++;
                for(col = pic_pgm.width/4; col<pic_pgm.width/2; col++) {
                    // Takes max grey value and subtracts by the current column number to get increasingly dark towards the right
                    gray_level = (int)((float)maximum_gray_level-((float)col_count) * ((float)maximum_gray_level / (float)(pic_pgm.width/4)));
                    // Mirror the gray value on each corner of the center
                    pic_pgm.image[row][col] = gray_level;
                    pic_pgm.image[(pic_pgm.height - 1) - row][col] = gray_level;
                    pic_pgm.image[row][(pic_pgm.width - 1) - col] = gray_level;
                    pic_pgm.image[(pic_pgm.height - 1) - row][(pic_pgm.width - 1) - col] = gray_level;
                    // Increases count proportionally with higher rows so it gets increasingly dark towards the bottom
                    if((row_count / slope)+(pic_pgm.width/4) > col) {
                        col_count++;
                    } 
                }
            }

            // Save image after setting pixels then cleanup 
            save_PGM_Image(&pic_pgm, output_image, image_format);
            free_PGM_Image(&pic_pgm);
            break;

        case PPM:
            // PPM image creation
            create_PPM_Image(&pic_ppm, image_width, image_height, maximum_gray_level); 
            proportion = ((float)pic_ppm.height / 2.0) / (float)maximum_gray_level;
            printf("%.5f\n", proportion);
            row_count = 0; // initialize
            for(row = 0; row<pic_ppm.height/2; row++) {            
                for(col = 0; col<pic_ppm.width/3; col++) {
                    pic_ppm.image[row][col][RED] = maximum_gray_level;
                    pic_ppm.image[row][col][GREEN] = (int)((float)row / proportion);
                    pic_ppm.image[row][col][BLUE] = (int)((float)row / proportion);
                }
                for(col = pic_ppm.width/3; col<(pic_ppm.width*2)/3; col++) {
                    pic_ppm.image[row][col][RED] = maximum_gray_level - (int)((float)row / proportion);;
                    pic_ppm.image[row][col][GREEN] = maximum_gray_level;
                    pic_ppm.image[row][col][BLUE] = maximum_gray_level - (int)((float)row / proportion);
                }
                for(col = (pic_ppm.width*2)/3; col<pic_ppm.width; col++) {
                    pic_ppm.image[row][col][RED] = (int)((float)row / proportion);
                    pic_ppm.image[row][col][GREEN] = (int)((float)row / proportion);
                    pic_ppm.image[row][col][BLUE] = maximum_gray_level;
                }
            }
            for(row = pic_ppm.height/2; row<pic_ppm.height; row++) {
                for(col = 0; col<pic_ppm.width/2; col++) {
                    pic_ppm.image[row][col][RED] = (int)((float)row_count / proportion);
                    pic_ppm.image[row][col][GREEN] = (int)((float)row_count / proportion);
                    pic_ppm.image[row][col][BLUE] = (int)((float)row_count / proportion);
                }
                for(col = pic_ppm.width/2; col<pic_ppm.width; col++) {
                    pic_ppm.image[row][col][RED] = maximum_gray_level - (int)((float)row_count / proportion);;
                    pic_ppm.image[row][col][GREEN] = maximum_gray_level - (int)((float)row_count / proportion);;
                    pic_ppm.image[row][col][BLUE] = maximum_gray_level - (int)((float)row / proportion);
                }
                row_count++;
            }

            save_PPM_Image(&pic_ppm, output_image, image_format);
            free_PPM_Image(&pic_ppm);
            break;

        default:
            // Invalid number should be caught in initial size check
            break;
    }
    return 0;
}

