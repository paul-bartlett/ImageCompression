#include "mean_absolute_error.h"

float mean_absolute_error(char *file_name_1_ptr, char *file_name_2_ptr) {

    int row, col, sum = 0;
    float scale, abs_error;
    struct PGM_Image pgmImage1, pgmImage2, *pgmImage;
    
    // Open the files
    load_PGM_Image(&pgmImage1, file_name_1_ptr);
    load_PGM_Image(&pgmImage2, file_name_2_ptr);

    // Check that images are the same size
    if(pgmImage1.height != pgmImage2.height || pgmImage1.width != pgmImage2.width) {
        printf("Images do not have the same dimensions\n");
        return 0;
    }

    // Check that images use the same gray value
    if(pgmImage1.maxGrayValue != pgmImage2.maxGrayValue) {
        // If pgm Image 2 has smaller gray value, divide for scale and multiply in loop
        if(pgmImage1.maxGrayValue > pgmImage2.maxGrayValue) {
            pgmImage = &pgmImage2;
            scale = (float) pgmImage1.maxGrayValue / pgmImage2.maxGrayValue;
            pgmImage2.maxGrayValue = pgmImage1.maxGrayValue;
        } else {
            pgmImage = &pgmImage1;
            scale = (float) pgmImage2.maxGrayValue / pgmImage1.maxGrayValue;
            pgmImage1.maxGrayValue = pgmImage2.maxGrayValue;
        }
        for(row = 0; row < pgmImage->height; row++)
            for(col = 0; col < pgmImage->width; col++)
                pgmImage->image[row][col] *= scale;
    }

    // Calculate the absolute sum of differences between the images
    for(row = 0; row < pgmImage1.height; row++)
        for(col = 0; col < pgmImage1.width; col++)
            sum += abs(pgmImage2.image[row][col] - pgmImage1.image[row][col]);
    
    abs_error = (float) sum / (row * col);
    return abs_error;
}

