#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "generate_huffman_nodes.h"

struct node {
    int first_value;
    int second_value;
}

struct node *generate_huffman_nodes(long int *pixel_frequency, int max_gray_value, int number_of_non_zero_values_in_the_frequency_array) {
    
    int non_zero = number_of_non_zero_values_in_the_frequency_array;
    int frequency_array[non_zero], freq_i = 0, i = 0, min = 0, second_min = 0;
    struct node *huffman_nodes = malloc((non_zero - 1) * sizeof(struct node));

    // Flatten large pixel frequency array to remove 0 values
    while(freq_i < non_zero) {
        if (pixel_frequency[i] > 0){
            frequency_array[freq_i] = pixel_frequency[i];
            freq_i++;
        }
        i++;
    }

    // Generate Huffman node pairs
    for(int node_i = 0; node_i < (non_zero - 1); node_i++) {
        for(int freq_i = 0; freq_i < non_zero; freq_i++) {
            // Record min index if new value is lower
            if (frequency[min] > frequency_array[freq_i]) {
                second_min = min;
                min = freq_i;
            }
        }


    }

	
