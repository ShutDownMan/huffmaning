#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stdio.h>

#define MAX_WORD_LENGTH 50
#define LINE_BUFFER_SIZE 1024

#define PARENT_AT(i) ((i - 1) / 2)
#define LEFT_CHILD_OF(i) (2 * i + 1)
#define RIGHT_CHILD_OF(i) (2 * i + 2)

/**
 * Structure to represent a huffman node
 */
typedef struct huffman_node {
  union {
    unsigned char c;
    char *word;
  } data;
  int freq;
} huffman_node;

/**
 * Structure to represent a huffman tree
 */
typedef struct huffman_tree {
  huffman_node **nodes;
} huffman_tree;

/**
 * Structure to represent a huffman header
 */
typedef struct huffman_header {
  unsigned int word_count;           // Number of words in the file
  unsigned int char_count;           // Number of characters in the file
  unsigned int word_freq_table_size; // Size of the word frequency table
  unsigned int char_freq_table_size; // Size of the character frequency table
  unsigned int word_table_size;      // Size of the word table
  unsigned int char_table_size;      // Size of the character table
  unsigned int compressed_size;      // Size of the compressed file
} huffman_header;


/**
 * Function to create a character code table from a huffman tree
 * @param input_file The input file
 * @param output_file The output file
 */
void huffman_encode_file_per_char(char *input_file, char *output_file);

/**
 * Function to create a word code table from a huffman tree
 * @param input_file The input file
 * @param output_file The output file
 */
void huffman_encode_file_per_word(char *input_file, char *output_file);

/**
 * Function to decompress a file using huffman decoding per character
 * @param input_file The input file
 * @param output_file The output file
 */
void huffman_decode_file_per_char(char *input_file, char *output_file);

/**
 * Function to decompress a file using huffman decoding per word
 * @param input_file The input file
 * @param output_file The output file
 */
void huffman_decode_file_per_word(char *input_file, char *output_file);

/**
 * Function to create a huffman tree from a character frequency table
 * @param char_freq_table The character frequency table
 * @return The huffman tree
 */
huffman_tree *huffman_create_tree_from_char_freq_table(int *char_freq_table);

/**
 * Function to create a huffman tree from a word frequency table
 * @param words The words
 * @param word_count The number of words
 * @param word_freq_table The word frequency table
 * @return The huffman tree
 */
huffman_tree *huffman_create_tree_from_word_freq_table(char **words, unsigned int word_count,
  int *word_freq_table);

/**
 * Function to delete a huffman tree from memory
 * @param tree The huffman tree
 */
void huffman_delete_tree(huffman_tree *tree);

/**
 * Function to print a huffman tree
 * @param tree The huffman tree
 * @param level The level of the tree
 */
void huffman_print_tree(huffman_node *tree, int level);

/**
 * Function to write a huffman character tree to a file
 * @param tree The huffman tree
 * @param output The output file
 */
void huffman_write_char_tree(huffman_tree *tree, FILE *output);

void huffman_write_char_tree_helper(huffman_node *node, FILE *output);

void huffman_write_word_tree(huffman_tree *tree, FILE *output);

void huffman_write_word_tree_helper(huffman_node *node, FILE *output);

huffman_tree *huffman_read_char_tree(FILE *input);

huffman_node *huffman_read_char_tree_helper(FILE *input);

huffman_node *huffman_create_node(char c, char *word, int freq,
                                  huffman_node *left, huffman_node *right);
#endif /* HUFFMAN_H */
