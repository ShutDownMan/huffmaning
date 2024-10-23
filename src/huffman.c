#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bitvector.h"
#include "huffman.h"
#include "priority_queue.h"

void huffman_encode_file_per_char(char *input_file, char *output_file) {
  // create a character frequency table
  int char_freq_table[256] = _huffman_get_char_freq_table_from_file(input_file);

  // create a huffman tree from the character frequency table
  huffman_tree *tree = huffman_create_tree_from_char_freq_table(char_freq_table);

  // create a character code table from the huffman tree
  trie *code_table = huffman_create_code_table(tree);

  // deallocate the character frequency table
  free(char_freq_table);
}

char *_huffman_get_char_freq_table_from_file(char *input_file) {
  // open the input file for reading
  FILE *input = fopen(input_file, "r");

  // if the file does not exist, return NULL
  if (input == NULL) {
    return NULL;
  }

  // allocate and initialize a character frequency table
  int *char_freq_table = calloc(256, sizeof(int));
  memset(char_freq_table, 0, 256 * sizeof(int));

  // read the file line by line and update the character frequency table
  char line[LINE_BUFFER_SIZE];
  while (fgets(line, LINE_BUFFER_SIZE, input) != NULL) {
    for (int i = 0; i < strlen(line); i++) {
      char_freq_table[(unsigned char)line[i]]++;
    }
  }

  // close the file
  fclose(input);

  return char_freq_table;
}