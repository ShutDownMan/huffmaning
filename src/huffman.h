#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stdio.h>

#include "trie.h"
#include "bitvector.h"
#include "priority_queue.h"
#include "dynamic_array.h"

#define MAX_WORD_LENGTH 50
#define LINE_BUFFER_SIZE 1024

#define PARENT_AT(i) ((i - 1) / 2)
#define LEFT_CHILD_OF(i) (2 * i + 1)
#define RIGHT_CHILD_OF(i) (2 * i + 2)

/**
 * Structure to represent a huffman node
 */
typedef struct huffman_node {
  char *data;
  int freq;
  struct huffman_node *left;
  struct huffman_node *right;
} huffman_node;

/**
 * Structure to represent a huffman tree
 */
typedef struct huffman_tree {
  huffman_node *root;
} huffman_tree;

/**
 * Structure to represent a huffman header
 */
typedef struct huffman_header {
  unsigned int root_offset;          // Index of the root node
  long word_list_offset;            // Offset of the word list
  long huffman_table_offset;        // Offset of the huffman table
  long compressed_offset;           // Offset of the compressed data
  unsigned int word_count;          // Number of words
} huffman_header;

/**
 * Function to create a character code table from a huffman tree
 * @param input_file The input file
 * @param output_file The output file
 */
void huffman_encode_file_per_char(char *input_file, char *output_file);

/**
 * Function to create a character frequency table from a file
 * @param input_file The input file
 * @return The character frequency table
 */
int *_huffman_get_char_freq_table_from_file(char *input_file);

/**
 * Function to create a word code table from a huffman tree
 * @param input_file The input file
 * @param output_file The output file
 */
void huffman_encode_file_per_word(char *input_file, char *output_file);

/**
 * Function to decompress a file using huffman decoding
 * @param input_file The input file
 * @param output_file The output file
 */
void huffman_decode_file(char *input_file, char *output_file);

char **_huffman_read_word_list(FILE *input);

huffman_tree *huffman_read_huffman_table(FILE *input);

void _huffman_populate_tree_with_words(huffman_tree *tree, FILE *input);

void _huffman_populate_tree_with_words_helper(huffman_node *node, FILE *input);

void huffman_decode_file_helper(FILE *input, FILE *output, huffman_header *header, huffman_tree *tree);

huffman_node *_huffman_read_huffman_table_helper(FILE *input);



/**
 * Function to create a huffman tree from a character frequency table
 * @param char_freq_table The character frequency table
 * @return The huffman tree
 */
huffman_tree *_huffman_create_tree_from_char_freq_table(int *char_freq_table);

/**
 * Function to create a huffman tree from a character frequency table
 * @param char_freq_table The character frequency table
 * @return The huffman tree
 */
trie *_huffman_char_create_code_table(huffman_tree *tree);

/**
 * Function to traverse a huffman tree to generate a character code table
 * @param tree The huffman tree
 * @param index The index of the node
 * @param code The code
 * @param depth The depth of the tree
 * @param code_table The code table
 */
void _huffman_char_traverse_tree(huffman_node *node, bitvector *code, int depth, trie *code_table);

/**
 * Function to encode a file using a code table
 * @param input_file The input file
 * @param output_file The output file
 * @param code_table The code table
 */
void huffman_encode_file(char *input_file, char *output_file, huffman_tree *tree, trie *code_table);

/**
 * Function to write a huffman header to a file
 * @param code_table The code table
 * @param input The input file
 * @param output The output file
 * @return The huffman header
 */
huffman_header *_huffman_write_header(huffman_tree *tree, trie *code_table, FILE *input, FILE *output);

/**
 * Function to write a word list to a file
 * @param code_table The code table
 * @param output The output file
 */
void _huffman_write_word_list(huffman_tree *tree, FILE *output);
void _huffman_write_word_list_helper(huffman_node *node, FILE *output);

/**
 * Function to write a code list to a file
 * @param code_table The code table
 * @param output The output file
 */
void _huffman_write_code_list(trie *code_table, FILE *output);

long _huffman_write_huffman_table(huffman_tree *tree, FILE *output);

long _huffman_write_huffman_table_helper(huffman_node *node, FILE *output);

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

void huffman_print(huffman_tree *tree);

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

#endif // HUFFMAN_H
