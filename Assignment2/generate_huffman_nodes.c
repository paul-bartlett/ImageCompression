#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "generate_huffman_nodes.h"

struct node *generate_huffman_nodes(long int *pixel_frequency, int max_gray_value, int number_of_non_zero_values_in_the_frequency_array) {
    
    int non_zero = number_of_non_zero_values_in_the_frequency_array;
    int frequency_map[non_zero], freq_i = 0, i = 0, min, second_min;
    struct node *huffman_nodes = malloc((non_zero - 1) * sizeof(struct node));

    // Create mapping to frequencies that are non-zero
    while(freq_i < non_zero) {
        if (pixel_frequency[i] > 0){
            frequency_map[freq_i] = i;
            freq_i++;
        }
        i++;
    }

    // Generate Huffman node pairs
    for(int node_i = 0; node_i < (non_zero - 1); node_i++) {
        min = 0; second_min = 0; // reset
        // Find two lowest values to combine as a pair
        for(int freq_i = 0; freq_i < non_zero; freq_i++) {
            // Record min index if new value is lower, and check second min
            if (pixel_frequency[frequency_map[freq_i]] == 0) {
                // pass if zeroed
            } else if (pixel_frequency[min] > pixel_frequency[frequency_map[freq_i]]) {
                second_min = min;
                min = frequency_map[freq_i];
            } else if (pixel_frequency[second_min] > pixel_frequency[frequency_map[freq_i]]) {
                second_min = frequency_map[freq_i];
            }
        }
        // Check if last one
        if(min == second_min) {
            printf("Error in last huffman pair generation\n");
        } else {
            // Combine values and zero one
            pixel_frequency[second_min] += pixel_frequency[min];
            pixel_frequency[min] = 0;
            
            // Record pair
            struct node huffman_node = { second_min, min };
            huffman_nodes[node_i] = huffman_node;
        }
    }
    return huffman_nodes;
}
	
