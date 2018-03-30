#include "DPCM_encoding_function.h"

void Encode_Using_DPCM(char *in_PGM_filename_Ptr, int prediction_rule, float *avg_abs_error_Ptr, float *std_abs_error_Ptr) {

    // Image variables
    struct PGM_Image pic_pgm;
    load_PGM_Image(&pic_pgm, in_PGM_filename_Ptr);
    int height = pic_pgm.height, width = pic_pgm.width, max_gray_value = pic_pgm.maxGrayValue, row, col, mode_flag;

    int prediction_array[height][width];
    int error_array[max_gray_value + 1];
    memset(error_array, 0, (max_gray_value + 1)*sizeof(int));
    int error_sum = 0;
    char *filename;

    // Check if valid file type
    if((filename = strrchr(in_PGM_filename_Ptr, '.')) != NULL) {
        if(strcmp(filename, ".pgm") != 0) {
            printf("File name given is not a .pgm file\n");
            return;
        }
    } else {
        printf("File name given has no extension (should be .pgm)\n");
        return;
    }

    // Write predicted values to a file
    char DPCM_filename[strlen(in_PGM_filename_Ptr) + 2 + 5];
    char errors_filename[strlen(in_PGM_filename_Ptr) + 2 + 7 + 4];

    sprintf(DPCM_filename, "%s.%d.DPCM", in_PGM_filename_Ptr, prediction_rule);
    sprintf(errors_filename, "%s.%d.errors.csv", in_PGM_filename_Ptr, prediction_rule);
    
    FILE *DPCM_file_pointer = fopen(DPCM_filename, "wb");
    if(DPCM_file_pointer == NULL) {
        printf("Error opening DPCM file for writing\n");
        return;
    }

    FILE *errors_file_pointer = fopen(errors_filename, "wb");
    if(errors_file_pointer == NULL) {
        printf("Error opening errors file for writing\n");
        return;
    }

    // Write the DPCM header
    fprintf(DPCM_file_pointer, "P2\n%d %d\n%d\n%d\n", width, height, max_gray_value, prediction_rule);

    // Predictions used regardless of rule
    prediction_array[0][0] = pic_pgm.image[0][0] - 128;
    fprintf(DPCM_file_pointer, "%d ", prediction_array[0][0]);

    // Top west row
    for(col = 1; col < width; col++) {
        prediction_array[0][col] =  pic_pgm.image[0][col] - pic_pgm.image[0][col-1];
        fprintf(DPCM_file_pointer, "%d ", prediction_array[0][col]);
    }
    
    // Second north row 
    for(col = 0; col < width; col++) {
        prediction_array[1][col] =  pic_pgm.image[1][col] - pic_pgm.image[0][col];
        fprintf(DPCM_file_pointer, "%d ", prediction_array[1][col]);
    }

    // First and second north columns
    for(row = 2; row < height; row++) {
        for(col = 0; col < 2; col++) {
            prediction_array[row][col] = pic_pgm.image[row][col] - pic_pgm.image[row-1][col];
            fprintf(DPCM_file_pointer, "%d ", prediction_array[row][col]);
        }
    }

    // Last north column
    for(row = 2; row < height; row++) {
        prediction_array[row][width-1] = pic_pgm.image[row][width-1] - pic_pgm.image[row-1][width-1];
        fprintf(DPCM_file_pointer, "%d ", prediction_array[row][width-1]);
    }

    // Prediction rule encoding: 1 for W, 2 for N, 3 for W/2 + N/2, 4 for CALIC
    switch(prediction_rule) {
        // W prediction
        case 1:
            for(row = 2; row < height; row++) {
                for(col = 2; col < (width - 1); col++) {
                    prediction_array[row][col] =  pic_pgm.image[row][col] - pic_pgm.image[row][col-1];
                    fprintf(DPCM_file_pointer, "%d ", prediction_array[row][col]);
                }
            }
            break;

        // N prediction
        case 2:
            for(row = 2; row < height; row++) {
                for(col = 2; col < (width - 1); col++) {
                    prediction_array[row][col] =  pic_pgm.image[row][col] - pic_pgm.image[row-1][col];
                    fprintf(DPCM_file_pointer, "%d ", prediction_array[row][col]);
                }   
            }
            break;

        // W/2 + N/2 prediction
        case 3:
            for(row = 2; row < height; row++) {
                for(col = 2; col < (width - 1); col++) {
                    prediction_array[row][col] =  pic_pgm.image[row][col] - ((pic_pgm.image[row][col-1] / 2) + (pic_pgm.image[row-1][col] / 2));
                    fprintf(DPCM_file_pointer, "%d ", prediction_array[row][col]);
                }
            }
            break;

        // CALIC prediction
        case 4:
            mode_flag = 1;
            for(row = 2; row < height; row++) {
                for(col = 2; col < (width - 1); col++) {
                    float prediction_fl;
                    int w = pic_pgm.image[row][col-1], 
                        ww = pic_pgm.image[row][col-2],
                        n = pic_pgm.image[row-1][col],
                        nw = pic_pgm.image[row-1][col-1],
                        ne = pic_pgm.image[row-1][col+1],
                        nn = pic_pgm.image[row-2][col],
                        nne = pic_pgm.image[row-2][col+1],
                        dh = abs(w - ww) + abs(n - nw) + abs(ne - n),
                        dv = abs(w - nw) + abs(n - nn) + abs(ne - nne),
                        mode = 1,
                        mode_check[6] = { ww, n, nw, ne, nn, nne },
                        binary = 1,
                        binary_check[2];

                        // Check if CALIC should be in binary mode or continuous mode
                        if(mode_flag) {
                            binary_check[0] = w;
                            binary = 1;
                            for(int i = 0; i < 6; i++) {
                                // Get both unique 
                                if(binary != 2) {
                                    if(mode_check[i] != binary_check[0]) {
                                        binary_check[1] = mode_check[i];
                                        binary++;
                                    }
                                // Exit binary mode if >2 values found
                                } else if(binary_check[0] != mode_check[i] && binary_check[1] != mode_check[i]) {
                                    if(mode == 0) {
                                        fprintf(DPCM_file_pointer, "2 "); // Send 2 to exit binary mode
                                        mode = 1;
                                    }
                                    mode_flag = 0;
                                    break;
                                }
                                // Start binary mode if loop is done
                                if(i == 5) {
                                    mode = 0;
                                }
                            }
                        }

                        // CALIC binary mode
                        if(mode == 0) {
                            printf("1");
                            // Match
                            if(pic_pgm.image[row][col] == w)
                                prediction_array[row][col] = 0;
                            else {
                                // Exit binary mode
                                if(pic_pgm.image[row][col] != binary_check[0] && pic_pgm.image[row][col] != binary_check[1]) {
                                    mode = 1;
                                    mode_flag = 0;
                                    fprintf(DPCM_file_pointer, "2 ");
                                // Other value
                                } else {
                                    prediction_array[row][col] = 1;
                                }
                            }
                        } 

                        // CALIC continuous-tone mode
                        if(mode == 1) {
                            if(dv - dh > 80) 
                                prediction_array[row][col] = w;
                            else if(dh - dv > 80) 
                                prediction_array[row][col] = n;
                            else {
                                prediction_fl = (w + n)/2.0 + (ne - nw)/4.0;
                                if(dv - dh > 32)
                                    prediction_array[row][col] = round((1.0/2.0) * prediction_fl + (1.0/2.0) * w);
                                else if(dh - dv > 32)
                                    prediction_array[row][col] = round((1.0/2.0) * prediction_fl + (1.0/2.0) * n);
                                else if(dv - dh > 8)
                                    prediction_array[row][col] = round((3.0/4.0) * prediction_fl + (1.0/4.0) * w);
                                else if(dh - dv > 8)
                                    prediction_array[row][col] = round((3.0/4.0) * prediction_fl + (1.0/4.0) * n);
                                else
                                    prediction_array[row][col] = round(prediction_fl);
                            }

                            prediction_array[row][col] =  pic_pgm.image[row][col] - prediction_array[row][col];
                        }
                        fprintf(DPCM_file_pointer, "%d ", prediction_array[row][col]);
                }
            }

            break;

        default:
            printf("Incorrect prediction rule used, must be from 1-4\n");
    }

    // Get the frequency and sum
    for(row = 0; row < height; row++) {
        for(col = 0; col < width; col++) {
            error_array[abs(prediction_array[row][col])]++; // frequency
            error_sum += abs(prediction_array[row][col]); // for mean and stdev
        }
    }

    // Write the error values and get sum for CSV and calculations
    for(int i = 0; i < max_gray_value+1; i++)
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

    error_std = sqrt(error_std / (height * width));

    // Save values in function parameters
    *avg_abs_error_Ptr = error_mean;
    *std_abs_error_Ptr = error_std;

    // Free memory
    free_PGM_Image(&pic_pgm);
}

