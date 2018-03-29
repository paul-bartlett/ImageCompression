#include "DPCM_encoding_function.h"

void Encode_Using_DPCM(char *in_PGM_filename_Ptr, int prediction_rule, float *avg_abs_error_Ptr, float *std_abs_error_Ptr) {

    // Image variables
    struct PGM_Image pic_pgm;
    load_PGM_Image(&pic_pgm, in_PGM_filename_Ptr);
    int height = pic_pgm.height, width = pic_pgm.width, max_gray_value = pic_pgm.maxGrayValue, row, col;

    int prediction_array[height][width];
    int *error_array = calloc(max_gray_value, sizeof(int)), error_sum = 0;

    // Predictions used regardless of rule
    prediction_array[0][0] = 128;

    // Top west row
    for(col = 1; col < width; col++)
        prediction_array[0][col] =  pic_pgm.image[0][col] - prediction_array[0][col-1];

    // Second north row 
    for(col = 0; col < width; col++)
        prediction_array[1][col] =  pic_pgm.image[1][col] - prediction_array[0][col];

    // First and second north columns
    for(row = 2; row < height; row++)
        for(col = 0; col < 2; col++) 
            prediction_array[row][col] = pic_pgm.image[row][col] - prediction_array[row-1][col];

    // Last north column
    for(row = 2; row < height; row++)
        prediction_array[row][width-1] = pic_pgm.image[row][width-1] - prediction_array[row-1][width-1];
    // Prediction rule encoding: 1 for W, 2 for N, 3 for W/2 + N/2, 4 for CALIC
    switch(prediction_rule) {
        // W prediction
        case 1:
            for(row = 2; row < height; row++)
                for(col = 2; col < (width - 1); col++)
                    prediction_array[row][col] =  pic_pgm.image[row][col] - prediction_array[row][col-1];

            break;

        case 2:

            break;

        case 3:

            break;

        case 4:

            break;

        default:
            printf("Incorrect prediction rule used, must be from 1-4\n");
    }

    // Write predicted values to a file
    char DPCM_filename[strlen(in_PGM_filename_Ptr) + 2 + 5];
    char errors_filename[strlen(in_PGM_filename_Ptr) + 2 + 7 + 4];

    sprintf(DPCM_filename, "%s.%d.DPCM", in_PGM_filename_Ptr, prediction_rule);
    sprintf(errors_filename, "%s.%d.errors.csv", in_PGM_filename_Ptr, prediction_rule);

    FILE *DPCM_file_pointer = fopen(DPCM_filename, "wb");
    if(DPCM_file_pointer == NULL) printf("Error opening DPCM file for writing\n");

    FILE *errors_file_pointer = fopen(errors_filename, "wb");
    if(errors_file_pointer == NULL) printf("Error opening errors file for writing\n");

    // Write the DPCM header
    fprintf(DPCM_file_pointer, "P2\n%d %d\n%d\n%d\n", width, height, max_gray_value, prediction_rule);

    // Write the prediction values and get frequency and sum
    for(row = 0; row < height; row++) {
        for(col = 0; col < width; col++) {
            fprintf(DPCM_file_pointer, "%d ", prediction_array[row][col]);
            error_array[abs(prediction_array[row][col])]++; // frequency
            error_sum += abs(prediction_array[row][col]); // for mean and stdev
        }
    }

    // Write the error values and get sum for CSV and calculations
    for(int i = 0; i < max_gray_value; i++)
        if(error_array[i] != 0)
            fprintf(errors_file_pointer, "%d,%d\n", i, error_array[i]);

    fclose(DPCM_file_pointer);
    fclose(errors_file_pointer);

    // Calculate the average and standard deviation of the errors
    float error_mean, error_std = 0.0;
    
    error_mean = (float) error_sum / (height * width);
 
    for(row = 0; row < height; row++) {
        for(col = 0; col < width; col++) {
            error_std += pow(abs(prediction_array[row][col]) - error_mean, 2);
        }
    }

    // Save values in function parameters
    *avg_abs_error_Ptr = error_mean;
    *std_abs_error_Ptr = error_std;

    // Free memory
    free_PGM_Image(&pic_pgm);
    free(error_array);
}

