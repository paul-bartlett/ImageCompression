#include "lz77_encoding_function.h"

void Encode_Using_LZ77(char *in_PGM_filename_Ptr, unsigned int searching_buffer_size,  float *avg_offset_Ptr, float *std_offset_Ptr, float *avg_length_Ptr, float *std_length_Ptr) {

    // Image variables
    struct PGM_Image pic_pgm;
    load_PGM_Image(&pic_pgm, in_PGM_filename_Ptr);

    int *pixel_array = malloc(pic_pgm.height * pic_pgm.width * sizeof(int)), pixel_count = 0;

    // Iterate through all pixels to flatten array
    for(int row = 0; row < pic_pgm.height; row++) {
        for(int col = 0; col < pic_pgm.width; col++) {
            pixel_array[pixel_count] = pic_pgm.image[row][col];
            pixel_count++;
        }
    }
    
    // LZ77 token arrays
    int *offset = calloc(pixel_count, sizeof(int)), 
        *matching_length = calloc(pixel_count, sizeof(int)), 
        *next_symbol = calloc(pixel_count, sizeof(int));

    int current_buffer_size = 0, matches, tok;
    
    // Iterate through all pixels to create all LZ77 tokens
    for(tok = 0; current_buffer_size < pixel_count; tok++) {
        // Buffer through previous values and search for matches
        for(int i = 1; i <= searching_buffer_size && i <= current_buffer_size; i++) {
            matches = 0;
            // Continue looping for more matches
            while(pixel_array[current_buffer_size + matches] == pixel_array[current_buffer_size - i + matches] && current_buffer_size + matches < pixel_count) {
                matches++;
            }
            // Update match length only if larger
            if(matches > matching_length[tok]) {
                matching_length[tok] = matches;
                offset[tok] = i;
            }
        }
        // Include matched characters when determining next symbol, then add that one to buffer size
        current_buffer_size += matching_length[tok];
        next_symbol[tok] = pixel_array[current_buffer_size];
        current_buffer_size++;
    }

    // Create files and file names
    int buffer_string_size = (int)(ceil(log10(searching_buffer_size))+1);
    char lzFileName[strlen(in_PGM_filename_Ptr) + buffer_string_size + 5];
    char offsetsFileName[strlen(in_PGM_filename_Ptr) + buffer_string_size + 14];
    char lengthsFileName[strlen(in_PGM_filename_Ptr) + buffer_string_size + 14];

    sprintf(lzFileName, "%s.%d.lz", in_PGM_filename_Ptr, searching_buffer_size);
    sprintf(offsetsFileName, "%s.%d.offsets.csv", in_PGM_filename_Ptr, searching_buffer_size);
    sprintf(lengthsFileName, "%s.%d.lengths.csv", in_PGM_filename_Ptr, searching_buffer_size);
   
    FILE *lzFilePointer = fopen(lzFileName, "wb");
    if(lzFilePointer == NULL) printf("Error opening lz file for writing");

    FILE *offsetsFilePointer = fopen(offsetsFileName, "wb");
    if(lzFilePointer == NULL) printf("Error opening offsets csv file for writing");

    FILE *lengthsFilePointer = fopen(lengthsFileName, "wb");
    if(lzFilePointer == NULL) printf("Error opening lengths csv file for writing");
    
    // Write the lz header
    fprintf(lzFilePointer, "P2\n%d %d\n%d\n%d %d\n", pic_pgm.width, pic_pgm.height, pic_pgm.maxGrayValue, searching_buffer_size, tok);

    int *offset_frequency = calloc(tok, sizeof(int));
    int *length_frequency = calloc(tok, sizeof(int));
    int offset_sum = 0, length_sum = 0;

    // Write the LZ77 arrays for offsets, matching lengths, and next symbols (and csv data)
    for(int i = 0; i < tok; i++) {
        fprintf(lzFilePointer, "%d ", offset[i]);
        offset_frequency[offset[i]]++;
    }

    for(int i = 0; i < tok; i++) {
        fprintf(lzFilePointer, "%d ", matching_length[i]);
        length_frequency[matching_length[i]]++;
    }

    for(int i = 0; i < tok; i++)
        fprintf(lzFilePointer, "%d ", next_symbol[i]);

    // CSV data
    for(int i = 0; i < tok; i++) {
        if(offset_frequency[i] != 0) {
            fprintf(offsetsFilePointer, "%d,%d\n", i, offset_frequency[i]);
        }
        if(length_frequency[i] != 0) {
            fprintf(lengthsFilePointer, "%d,%d\n", i, length_frequency[i]);
        }
        offset_sum += offset[i]; // for mean and stdev
        length_sum += matching_length[i]; // for mean and stdev
    }

    fclose(lzFilePointer);
    fclose(offsetsFilePointer);
    fclose(lengthsFilePointer);

    // Calculate the average and standard deviation of the offsets and match lengths
    float offset_mean, offset_stdev = 0.0, length_mean, length_stdev = 0.0;
    
    offset_mean = (float) offset_sum / tok;
    length_mean = (float) length_sum / tok;

    for(int i = 0; i < tok; i++) {
        offset_stdev += pow(offset[i] - offset_mean, 2);
        length_stdev += pow(matching_length[i] - length_mean, 2);
    }

    offset_stdev = sqrt(offset_stdev / tok);
    length_stdev = sqrt(length_stdev / tok);

    // Save values in function parameters
    *avg_offset_Ptr = offset_mean;
    *avg_length_Ptr = length_mean;
    *std_offset_Ptr = offset_stdev;
    *std_length_Ptr = length_stdev;

    // Free memory
    free_PGM_Image(&pic_pgm);
    free(pixel_array);
    free(offset);
    free(matching_length);
    free(next_symbol);
    free(offset_frequency);
    free(length_frequency);
}

