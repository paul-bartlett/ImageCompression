#ifndef GENERATE_HUFFMAN_NODES_H
#define GENERATE_HUFFMAN_NODES_H

struct node {
    int first_value;
    int second_value;
};

struct node *generate_huffman_nodes(long int *pixel_frequency, int max_gray_value, int number_of_non_zero_values_in_the_frequency_array);

#endif /* GENERATE_HUFFMAN_NODES_H */
