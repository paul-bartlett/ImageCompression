#ifndef HUFFMAN_ENCODE_IMAGE_H
#define HUFFMAN_ENCODE_IMAGE_H

unsigned char *huffman_encode_image(struct PGM_Image *input_pgm_image, struct node *huffman_node, int number_of_nodes, unsigned long int *length_of_encoded_array);

#endif /* HUFFMAN_ENCODE_IMAGE_H */
