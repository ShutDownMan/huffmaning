#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stdio.h>

#define MAX_WORD_LENGTH 50

// Structure for a huffman node
typedef struct huffman_node {
    unsigned char c; // Character for leaf nodes
    char *word; // Word for internal nodes
    int freq; // Frequency of the character or sum of frequencies for internal nodes
    struct huffman_node *left; // Pointer to left child
    struct huffman_node *right; // Pointer to right child
} huffman_node;

// Structure for a huffman tree
typedef struct huffman_tree {
    huffman_node *root; // Pointer to the root node of the tree
} huffman_tree;

// Structure for a huffman header
typedef struct huffman_header {
    unsigned int word_count; // Number of words in the file
    unsigned int char_count; // Number of characters in the file
    unsigned int word_freq_table_size; // Size of the word frequency table
    unsigned int char_freq_table_size; // Size of the character frequency table
    unsigned int word_table_size; // Size of the word table
    unsigned int char_table_size; // Size of the character table
    unsigned int compressed_size; // Size of the compressed file
} huffman_header;

// Function to create a huffman tree from a character frequency table
huffman_tree* huffman_create_tree_from_char_freq_table(int *char_freq_table);

// Function to create a huffman tree from a word frequency table
huffman_tree* huffman_create_tree_from_word_freq_table(char **words, unsigned int word_count, int *word_freq_table);

// Function to delete a huffman tree
void huffman_delete_tree(huffman_tree *tree);

// Function to print a huffman tree
void huffman_print_tree(huffman_node *tree, int level);

// Function to compress a file using huffman encoding per character
void huffman_encode_file_per_char(char *input_file, char *output_file);

// Function to compress a file using huffman encoding per word
void huffman_encode_file_per_word(char *input_file, char *output_file);

// Function to decompress a file using huffman decoding per character
void huffman_decode_file_per_char(char *input_file, char *output_file);

// Function to decompress a file using huffman decoding per word
void huffman_decode_file_per_word(char *input_file, char *output_file);

void huffman_write_char_tree(huffman_tree *tree, FILE *output);

void huffman_write_char_tree_helper(huffman_node *node, FILE *output);

void huffman_write_word_tree(huffman_tree *tree, FILE *output);

void huffman_write_word_tree_helper(huffman_node *node, FILE *output, int *index);

huffman_tree *huffman_read_char_tree(FILE *input);

huffman_node *huffman_read_char_tree_helper(FILE *input);

huffman_node *huffman_create_node(char c, int freq, huffman_node *left, huffman_node *right);

#endif /* HUFFMAN_H */
