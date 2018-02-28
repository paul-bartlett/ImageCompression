#ifndef HUFFMAN_DECODE_IMAGE_H
#define HUFFMAN_DECODE_IMAGE_H

struct PGM_Image *huffman_decode_image( int image_width, int image_height, int max_gray_value, int number_of_nodes, struct node *huffman_node, long int length_of_encoded_image, unsigned char *encoded_image);

#endif /* HUFFMAN_DECODE_IMAGE_H */
