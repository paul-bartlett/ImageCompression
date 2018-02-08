#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "libpnm.h"

#define PBM 1
#define PGM 2
#define PPM 3

int main(int argc, char *argv[])  { 

    int row, col, col_count, row_count, gray_level, maximum_gray_level = 255;
    float slope, proportion;
    struct PBM_Image pic_pbm;
    struct PGM_Image pic_pgm, pic_pgm_red, pic_pgm_green, pic_pgm_blue;
    struct PPM_Image pic_ppm;

    if(argc != 6) {
        printf("You must supply 5 arguments: image type code (1 for pbm, 2 for pgm, or 3 for ppm), image width, image height, output image name, and image format (0 for ASCII, 1 for raw)\n");
        return 0;
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
            return 0;
        }
    } else if(image_type == PPM) {
        if(image_width >= 6 && image_width % 6 == 0 && image_height >= 4 && image_height % 4 == 0)
            printf("Good image size\n");
        else {
            printf("Width must be greater or equal to 6 and divisible by 6, and height must be greater or equal to 4 and divisible by 4\n");
            return 0;
        }
    } else {
        printf("Please enter a valid image type (1-3)\n");
        return 0;
    }   
    // Check for valid image format
    if(image_format != 0 && image_format != 1) {
        printf("Please enter a valid image format (0 for ASCII, 1 for raw)\n");
        return 0;
    }

    // Decide which of the 3 image types to create (PBM, PGM, PPM)
    switch(image_type) {

        case PBM:
            // PBM image creation
            create_PBM_Image(&pic_pbm, image_width, image_height); 

            // Iterate through all pixels and set pixel white if in the middle, else set to black
            for(row = 0; row<pic_pbm.height; row++) {
                for(col = 0; col<pic_pbm.width; col++) {
                    // Change pixel to white in the middle, else black
                    if(row >= pic_pbm.height/4 && row < (pic_pbm.height*3)/4 && col >= pic_pbm.width/4 && col < (pic_pbm.width*3)/4)
                        pic_pbm.image[row][col] = 0; // white
                    else
                        pic_pbm.image[row][col] = 1; // black
                }
            }

            // Calculate slope for the corner to corner black pixels
            slope = (float)pic_pbm.height / (float)pic_pbm.width;

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
                    // Change pixel to white in the middle, else black
                    if(row >= pic_pgm.height/4 && row < (pic_pgm.height*3)/4 && col >= pic_pgm.width/4 && col < (pic_pgm.width*3)/4)
                        pic_pgm.image[row][col] = 255; // white
                    else
                        pic_pgm.image[row][col] = 0; // black
                }
            }

            // Calculate slope for the increase in darkness towards the bottom
            slope = (float)pic_pgm.height / (float)pic_pgm.width;
            
            // Iterate by column if height is greater for proper proportional grey colour, else iterate by row
            if(pic_pgm.height > pic_pgm.width) {
                // Creates middle of image that gets increasingly dark towards the center
                col_count = 0; // initialize
                for(col = pic_pgm.width/4; col<pic_pgm.width/2; col++) {            
                    row_count = 0;
                    col_count++;
                    for(row = pic_pgm.height/4; row<pic_pgm.height/2; row++) {
                        // Takes max grey value and subtracts by the proportional height to get increasingly dark towards the bottom
                        gray_level = (int)((float)maximum_gray_level-((float)row_count) * ((float)maximum_gray_level / ((float)(pic_pgm.height/4) - 1.0)));
                        // Mirror the gray value on each corner of the center portion
                        pic_pgm.image[row][col] = gray_level;
                        pic_pgm.image[(pic_pgm.height - 1) - row][col] = gray_level;
                        pic_pgm.image[row][(pic_pgm.width - 1) - col] = gray_level;
                        pic_pgm.image[(pic_pgm.height - 1) - row][(pic_pgm.width - 1) - col] = gray_level;
                        // Increases count proportionally with higher rows so it gets increasingly dark towards the right
                        if((col_count*slope)+(pic_pgm.height/4 - 1) > row) {
                            row_count++;
                        } 
                    }
                }
            } else {
                // Creates middle of image that gets increasingly dark towards the center
                row_count = 0; // initialize
                for(row = pic_pgm.height/4; row<pic_pgm.height/2; row++) {            
                    col_count = 0;
                    row_count++;
                    for(col = pic_pgm.width/4; col<pic_pgm.width/2; col++) {
                        // Takes max grey value and subtracts by the proportional width to get increasingly dark towards the right
                        gray_level = (int)((float)maximum_gray_level-((float)col_count) * ((float)maximum_gray_level / ((float)(pic_pgm.width/4) - 1.0)));
                        // Mirror the gray value on each corner of the center portion
                        pic_pgm.image[row][col] = gray_level;
                        pic_pgm.image[(pic_pgm.height - 1) - row][col] = gray_level;
                        pic_pgm.image[row][(pic_pgm.width - 1) - col] = gray_level;
                        pic_pgm.image[(pic_pgm.height - 1) - row][(pic_pgm.width - 1) - col] = gray_level;
                        // Increases count proportionally with higher rows so it gets increasingly dark towards the bottom
                        if((row_count/slope)+(pic_pgm.width/4 - 1) > col) {
                            col_count++;
                        } 
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
            // proportion of colour for half the height of the image
            proportion = (((float)pic_ppm.height / 2.0) - 1) / (float)maximum_gray_level;
            row_count = 0; // initialize
            // Loops for the top half of the image to fill in each colour gradient
            for(row = 0; row<pic_ppm.height/2; row++) {
                // Fills in red gradient
                for(col = 0; col<pic_ppm.width/3; col++) {
                    pic_ppm.image[row][col][RED] = maximum_gray_level;
                    pic_ppm.image[row][col][GREEN] = (int)((float)row / proportion);
                    pic_ppm.image[row][col][BLUE] = (int)((float)row / proportion);
                }
                // Fills in green gradient
                for(col = pic_ppm.width/3; col<(pic_ppm.width*2)/3; col++) {
                    pic_ppm.image[row][col][RED] = maximum_gray_level - (int)((float)row / proportion);
                    pic_ppm.image[row][col][GREEN] = maximum_gray_level;
                    pic_ppm.image[row][col][BLUE] = maximum_gray_level - (int)((float)row / proportion);
                }
                // Fills in blue gradient
                for(col = (pic_ppm.width*2)/3; col<pic_ppm.width; col++) {
                    pic_ppm.image[row][col][RED] = (int)((float)row / proportion);
                    pic_ppm.image[row][col][GREEN] = (int)((float)row / proportion);
                    pic_ppm.image[row][col][BLUE] = maximum_gray_level;
                }
            }
            // Loops for the bottom half of the image to fill in both black and white gradients
            for(row = pic_ppm.height/2; row<pic_ppm.height; row++) {
                // Gradient with black at top and white at bottom
                for(col = 0; col<pic_ppm.width/2; col++) {
                    pic_ppm.image[row][col][RED] = (int)((float)row_count / proportion);
                    pic_ppm.image[row][col][GREEN] = (int)((float)row_count / proportion);
                    pic_ppm.image[row][col][BLUE] = (int)((float)row_count / proportion);
                }
                // Gradient with white at top and black at bottom
                for(col = pic_ppm.width/2; col<pic_ppm.width; col++) {
                    pic_ppm.image[row][col][RED] = maximum_gray_level - (int)((float)row_count / proportion);
                    pic_ppm.image[row][col][GREEN] = maximum_gray_level - (int)((float)row_count / proportion);
                    pic_ppm.image[row][col][BLUE] = maximum_gray_level - (int)((float)row_count / proportion);
                }
                row_count++;
            }
            // Copy each color to a gray map then save and free memory
            copy_PPM_to_PGM(&pic_ppm, &pic_pgm_red, RED);
            copy_PPM_to_PGM(&pic_ppm, &pic_pgm_green, GREEN);
            copy_PPM_to_PGM(&pic_ppm, &pic_pgm_blue, BLUE);

            // Create new file names for each colour grey map and save
            char * file_name_r = malloc(strlen(output_image) + 2 + 1); // 2 extra for colour identifier
            char * file_name_g = malloc(strlen(output_image) + 2 + 1); 
            char * file_name_b = malloc(strlen(output_image) + 2 + 1); 
            strcpy(file_name_r, "R_");
            strcpy(file_name_g, "G_");
            strcpy(file_name_b, "B_");
            strcat(file_name_r, output_image);
            strcat(file_name_g, output_image);
            strcat(file_name_b, output_image);
            save_PPM_Image(&pic_ppm, output_image, image_format);
            save_PGM_Image(&pic_pgm_red, file_name_r, image_format);
            save_PGM_Image(&pic_pgm_green, file_name_g, image_format);
            save_PGM_Image(&pic_pgm_blue, file_name_b, image_format);

            // Clean up memory
            free(file_name_r);
            free(file_name_g);
            free(file_name_b);
            free_PPM_Image(&pic_ppm);
            free_PGM_Image(&pic_pgm_red);
            free_PGM_Image(&pic_pgm_green);
            free_PGM_Image(&pic_pgm_blue);
            break;

        default:
            // Invalid image type should be caught in initial size check
            break;
    }
    return 0;
}

