#include "DPCM_decoding_function.h"

void Decode_Using_DPCM (char *in_filename_Ptr) {
    
    char c;
    int row, col, width, height, max_gray_value, prediction_rule, mode_flag;
    struct PGM_Image pic_pgm;

    // Open file for reading
    FILE *DPCM_file_pointer =  fopen(in_filename_Ptr, "rb");
    if(DPCM_file_pointer == NULL) printf("Error opening DPCM file for reading");

    // Make sure the first char is P
    if(fgetc(DPCM_file_pointer) != 'P') { 
        printf("Invalid PGM image: missing P");
        fclose(DPCM_file_pointer);
    }

    // Make sure the second char is either a 2 or 5
    c = fgetc(DPCM_file_pointer);
    if(c != '2' && c != '5') { 
        printf("Invalid PGM image: missing 2 or 5");
        fclose(DPCM_file_pointer);
    }

    // Get the width, height, max gray value, and prediction rule of the image
    width = geti(DPCM_file_pointer); 
    height = geti(DPCM_file_pointer);
    max_gray_value = geti(DPCM_file_pointer);
    prediction_rule = geti(DPCM_file_pointer);

    create_PGM_Image(&pic_pgm, width, height, max_gray_value);

    // Get prediction values from file in the order they were put in
    int prediction_array[height][width];

    // Predictions used regardless of rule
    fscanf(DPCM_file_pointer, "%d ", &prediction_array[0][0]);
    pic_pgm.image[0][0] = prediction_array[0][0] + 128;

    // Top west row
    for(col = 1; col < width; col++) {
        fscanf(DPCM_file_pointer, "%d ", &prediction_array[0][col]);
        pic_pgm.image[0][col] = prediction_array[0][col] + pic_pgm.image[0][col-1];
    }

    // Second north row 
    for(col = 0; col < width; col++) {
        fscanf(DPCM_file_pointer, "%d ", &prediction_array[1][col]);
        pic_pgm.image[1][col] = prediction_array[1][col] + pic_pgm.image[0][col];
    }

    // First and second north columns
    for(row = 2; row < height; row++) {
        for(col = 0; col < 2; col++) {
            fscanf(DPCM_file_pointer, "%d ", &prediction_array[row][col]);
            pic_pgm.image[row][col] = prediction_array[row][col] + pic_pgm.image[row-1][col];
        }
    }

    // Last north column
    for(row = 2; row < height; row++) {
        fscanf(DPCM_file_pointer, "%d ", &prediction_array[row][width-1]);
        pic_pgm.image[row][width-1] = prediction_array[row][width-1] + pic_pgm.image[row-1][width-1];
    }
    
    // Prediction rule decoding: 1 for W, 2 for N, 3 for W/2 + N/2, 4 for CALIC
    switch(prediction_rule) {
        // W prediction
        case 1:
            for(row = 2; row < height; row++) {
                for(col = 2; col < (width - 1); col++) {
                    fscanf(DPCM_file_pointer, "%d ", &prediction_array[row][col]);
                    pic_pgm.image[row][col] = prediction_array[row][col] + pic_pgm.image[row][col-1];
                }
            }
            break;

            // N prediction
        case 2:
            for(row = 2; row < height; row++) {
                for(col = 2; col < (width - 1); col++) {
                    fscanf(DPCM_file_pointer, "%d ", &prediction_array[row][col]);
                    pic_pgm.image[row][col] = prediction_array[row][col] + pic_pgm.image[row-1][col];
                }   
            }
            break;

            // W/2 + N/2 prediction
        case 3:
            for(row = 2; row < height; row++) {
                for(col = 2; col < (width - 1); col++) {
                    fscanf(DPCM_file_pointer, "%d ", &prediction_array[row][col]);
                    pic_pgm.image[row][col] = prediction_array[row][col] + ((pic_pgm.image[row][col-1] / 2) + (pic_pgm.image[row-1][col] / 2));
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
                        binary_check[2],
                        prediction;
                    
                        fscanf(DPCM_file_pointer, "%d ", &prediction_array[row][col]);

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
                                        mode = 1;
                                        fscanf(DPCM_file_pointer, "%d ", &prediction_array[row][col]);
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
                            // Match
                            if(prediction_array[row][col] == 0)
                                pic_pgm.image[row][col] = w;
                            // Other value
                            else if(prediction_array[row][col] == 1) {
                                if(w != binary_check[0])
                                    pic_pgm.image[row][col] = binary_check[0];
                                else
                                    pic_pgm.image[row][col] = binary_check[1];
                            // Exit binary mode
                            } else if(prediction_array[row][col] == 2) {
                                mode = 1;
                                mode_flag = 0;
                                fscanf(DPCM_file_pointer, "%d ", &prediction_array[row][col]);
                            }
                        } 

                        // CALIC continuous-tone mode
                        if(mode == 1) {
                            if(dv - dh > 80) 
                                prediction = w;
                            else if(dh - dv > 80) 
                                prediction = n;
                            else {
                                prediction_fl = (w + n)/2.0 + (ne - nw)/4.0;
                                if(dv - dh > 32)
                                    prediction = round((1.0/2.0) * prediction_fl + (1.0/2.0) * w);
                                else if(dh - dv > 32)
                                    prediction = round((1.0/2.0) * prediction_fl + (1.0/2.0) * n);
                                else if(dv - dh > 8)
                                    prediction = round((3.0/4.0) * prediction_fl + (1.0/4.0) * w);
                                else if(dh - dv > 8)
                                    prediction = round((3.0/4.0) * prediction_fl + (1.0/4.0) * n);
                                else
                                    prediction = round(prediction_fl);
                            }

                            pic_pgm.image[row][col] = prediction + prediction_array[row][col];
                        }
                }
            }

            break;

        default:
            printf("Incorrect prediction rule used, must be from 1-4\n");
    }

    fclose(DPCM_file_pointer);

    // Save image
    char DPCM_filename[strlen(in_filename_Ptr) + 5];
    sprintf(DPCM_filename, "%s.pgm", in_filename_Ptr);
    save_PGM_Image(&pic_pgm, DPCM_filename, 0);

    // Free memory
    free_PGM_Image(&pic_pgm);
}

