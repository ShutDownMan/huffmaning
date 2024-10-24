#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "huffman.h"

void huffman_encode_file_per_char(char *input_file, char *output_file) {
  // create a character frequency table
  int *char_freq_table = _huffman_get_char_freq_table_from_file(input_file);

  // create a huffman tree from the character frequency table
  huffman_tree *tree = _huffman_create_tree_from_char_freq_table(char_freq_table);

  // printf("Huffman tree:\n");
  // huffman_print(tree);
  // printf("\n====================\n");

  // create a character code table from the huffman tree
  trie *code_table = _huffman_char_create_code_table(tree);

  // trie_print(code_table);

  // deallocate the character frequency table
  free(char_freq_table);

  // encode the input file using the character code table
  huffman_encode_file(input_file, output_file, tree, code_table);
}

int *_huffman_get_char_freq_table_from_file(char *input_file) {
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

int _freq_compare(const void *key1, const void *key2) {
  return ((huffman_node *)key2)->freq - ((huffman_node *)key1)->freq;
}

void huffman_node_print(void *data) {
  huffman_node *node = (huffman_node *)data;
  if (node->data != NULL) {
    printf("%s (%d)\n", node->data, node->freq);
  } else {
    printf("  (%d)\n", node->freq);
  }
}

huffman_tree *_huffman_create_tree_from_char_freq_table(int *char_freq_table) {
  // create a priority queue
  priority_queue *queue = priority_queue_create(NULL, _freq_compare);

  // insert the characters into the priority queue
  for (int i = 0; i < 256; i++) {
    if (char_freq_table[i] > 0) {
      huffman_node *node = malloc(sizeof(huffman_node));
      node->data = malloc(sizeof(char) * 2);
      node->data[0] = (unsigned char)i;
      node->data[1] = '\0';
      node->freq = char_freq_table[i];
      node->left = NULL;
      node->right = NULL;
      priority_queue_insert(queue, node);
    }
  }

  // build the huffman tree
  unsigned int current_index = 0;
  while (priority_queue_size(queue) > 1) {
    huffman_node *left = NULL, *right = NULL, *parent = NULL;

    // extract the two nodes with the lowest frequency
    priority_queue_extract(queue, (void **)&left);
    priority_queue_extract(queue, (void **)&right);

    // printf("casando %s (%d) com %s (%d)\n", left->data, left->freq, right->data, right->freq);

    // create a parent node
    parent = malloc(sizeof(huffman_node));
    parent->data = NULL;
    parent->freq = left->freq + right->freq;
    parent->left = left;
    parent->right = right;

    // printf("node freq: %d\n", parent->freq);

    // insert the parent node back into the priority queue
    priority_queue_insert(queue, parent);
  }

  // extract the root node from the priority queue
  huffman_node *root;
  priority_queue_extract(queue, (void **)&root);

  // deallocate the priority queue
  priority_queue_destroy(queue);

  // create a huffman tree
  huffman_tree *tree = malloc(sizeof(huffman_tree));
  tree->root = root;

  return tree;
}

void huffman_print(huffman_tree *tree) {
  huffman_print_tree(tree->root, 0);
}

void huffman_print_tree(huffman_node *node, int level) {
  if (node == NULL) {
    return;
  }

  for (int i = 0; i < level; i++) {
    printf("  ");
  }

  if (node->data != NULL) {
    printf("%s (%d)\n", node->data, node->freq);
  } else {
    printf("%d\n", node->freq);
  }

  huffman_print_tree(node->left, level + 1);
  huffman_print_tree(node->right, level + 1);
}

trie *_huffman_char_create_code_table(huffman_tree *tree) {
  // create a trie to store the character code table
  trie *code_table = trie_create();

  // bitvector to store the character code
  bitvector *code = bitvector_create(0);

  // traverse the huffman tree to generate the character code table
  _huffman_char_traverse_tree(tree->root, code, 0, code_table);

  return code_table;
}

void _huffman_char_traverse_tree(huffman_node *node, bitvector *code, int depth, trie *code_table) {
  if (node == NULL) {
    bitvector_destroy(code);
    return;
  }

  if (node->data != NULL) {
    // insert the character code into the code table
    bitvector *copy = bitvector_copy(code);
    trie_insert(code_table, node->data, copy);
  } else {
    // traverse the left subtree
    // bitvector_set(code, depth, 0);
    bitvector *copy_left = bitvector_copy(code);
    bitvector_append(copy_left, 0);
    _huffman_char_traverse_tree(node->left, copy_left, depth + 1, code_table);

    // traverse the right subtree
    // bitvector_set(code, depth, 1);
    bitvector *copy_right = bitvector_copy(code);
    bitvector_append(copy_right, 1);
    _huffman_char_traverse_tree(node->right, copy_right, depth + 1, code_table);
  }
}

void huffman_encode_file(char *input_file, char *output_file, huffman_tree *tree, trie *code_table) {
  // open the input file for reading
  FILE *input = fopen(input_file, "r");

  // if the file does not exist, return
  if (input == NULL) {
    return;
  }

  // open the output file for writing
  FILE *output = fopen(output_file, "wb");

  // if the file does not exist, return
  if (output == NULL) {
    fclose(input);
    return;
  }

  // write the header to the output file
  huffman_header *header = _huffman_write_header(tree, code_table, input, output);

  bitvector *output_buffer = bitvector_create(0);

  // encode the input file using the character code table
  char line[LINE_BUFFER_SIZE];
  unsigned int word_count = 0;
  fseek(input, 0, SEEK_SET);
  fseek(output, header->compressed_offset, SEEK_SET);
  while (fgets(line, LINE_BUFFER_SIZE, input) != NULL) {
    int steps = 0;
    for (int i = 0; i < strlen(line); i += steps) {
      bitvector *code = trie_search(code_table, (char *)(line + i), &steps, false);
      // printf("buff: %s\n", (char *)(line + i));
      // printf("steps: %d\n", steps);

      if (steps == 0) {
        printf("could not find code for '%s'\n", (char *)(line + i));
        steps = 1;
      }

      if (code == NULL) {
        printf("code not found\n");
        continue;
      }

      // printf("code: ");
      // bitvector_print(code);
      bitvector_concat(output_buffer, code);
      word_count++;
    }
  }

  header->word_count = word_count;
  // printf("word_count: %d\n", word_count);

  // write the output buffer to the output file
  // bitvector *compressed_output_buffer = bitvector_compress(output_buffer);
  // bitvector_print(output_buffer);
  // bitvector_print(compressed_output_buffer);
  fwrite(output_buffer->bits, output_buffer->size / 8 + 1, 1, output);

  // write the huffman header to the output file
  fseek(output, 0, SEEK_SET);
  fwrite(header, sizeof(huffman_header), 1, output);

  // close the files
  fclose(input);
  fclose(output);
}

huffman_header *_huffman_write_header(huffman_tree *tree, trie *code_table, FILE *input, FILE *output) {
  // create a huffman header
  huffman_header *header = malloc(sizeof(huffman_header));
  header->root_offset = 0;
  header->word_list_offset = 0;
  header->huffman_table_offset = 0;
  header->compressed_offset = 0;

  // write the huffman header to the output file
  fwrite(&header, sizeof(huffman_header), 1, output);

  header->word_list_offset = ftell(output);

  // write the word list to the output file
  _huffman_write_word_list(tree, output);

  header->huffman_table_offset = ftell(output);

  // write the code list to the output file
  header->root_offset = _huffman_write_huffman_table(tree, output);

  header->compressed_offset = ftell(output);

  // write the huffman header to the output file
  fseek(output, 0, SEEK_SET);
  fwrite(header, sizeof(huffman_header), 1, output);

  return header;
}

void _huffman_write_word_list(huffman_tree *tree, FILE *output) {
  // recursively write the word list to the output file
  _huffman_write_word_list_helper(tree->root, output);
}

void _huffman_write_word_list_helper(huffman_node *node, FILE *output) {
  if (node == NULL) {
    return;
  }

  if (node->data != NULL) {
    int current_offset = ftell(output);
    int length = strlen(node->data);
    // printf("writing %s (%d)\n", node->data, length);
    fwrite(&length, sizeof(int), 1, output);
    fwrite(node->data, length, 1, output);
    free(node->data);
    node->data = (char *)current_offset;
  } else {
    node->data = (char *)-1;
    _huffman_write_word_list_helper(node->left, output);
    _huffman_write_word_list_helper(node->right, output);
  }
}

long _huffman_write_huffman_table(huffman_tree *tree, FILE *output) {
  // write the huffman table to the output file
  return _huffman_write_huffman_table_helper(tree->root, output);
}

long _huffman_write_huffman_table_helper(huffman_node *node, FILE *output) {
  if (node == NULL) {
    return -1;
  }

  int freq = node->freq;
  // already has offset as data
  long data = (long)node->data;
  long left_offset = _huffman_write_huffman_table_helper(node->left, output);
  long right_offset = _huffman_write_huffman_table_helper(node->right, output);

  long current_offset = ftell(output);

  fwrite(&node->freq, sizeof(int), 1, output);
  fwrite(&node->data, sizeof(long), 1, output);
  fwrite(&left_offset, sizeof(long), 1, output);
  fwrite(&right_offset, sizeof(long), 1, output);

  return current_offset;
}

void huffman_decode_file(char *input_file, char *output_file) {
  // open the input file for reading
  FILE *input = fopen(input_file, "rb");

  // if the file does not exist, return
  if (input == NULL) {
    return;
  }

  // open the output file for writing
  FILE *output = fopen(output_file, "w");

  // if the file does not exist, return
  if (output == NULL) {
    fclose(input);
    return;
  }

  // read the huffman header from the input file
  huffman_header header;
  fseek(input, 0, SEEK_SET);
  fread(&header, sizeof(huffman_header), 1, input);

  // read the huffman table from the input file
  fseek(input, header.root_offset, SEEK_SET);
  huffman_tree *tree = huffman_read_huffman_table(input);

  // populate the words from the input file
  fseek(input, header.word_list_offset, SEEK_SET);
  _huffman_populate_tree_with_words(tree, input);

  // huffman_print(tree);

  // decode the input file using the huffman tree
  fseek(input, header.compressed_offset, SEEK_SET);
  // printf("compressed_offset: 0x%x\n", header.compressed_offset);
  huffman_decode_file_helper(input, output, &header, tree);

  // close the files
  fclose(input);
  fclose(output);
}

char **_huffman_read_word_list(FILE *input) {
  // read the number of words from the input file
  unsigned int word_count;
  fread(&word_count, sizeof(unsigned int), 1, input);

  // allocate memory for the words
  char **words = malloc(word_count * sizeof(char *));

  // read the words from the input file
  for (int i = 0; i < word_count; i++) {
    unsigned int length;
    fread(&length, sizeof(unsigned int), 1, input);
    words[i] = malloc(length + 1);
    fread(words[i], sizeof(char), length, input);
    words[i][length] = '\0';
  }

  return words;
}

huffman_tree *huffman_read_huffman_table(FILE *input) {
  // read the huffman table from the input file
  huffman_node *root = _huffman_read_huffman_table_helper(input);

  // create a huffman tree
  huffman_tree *tree = malloc(sizeof(huffman_tree));
  tree->root = root;

  return tree;
}

huffman_node *_huffman_read_huffman_table_helper(FILE *input) {
  // read the node from the input file
  huffman_node *node = malloc(sizeof(huffman_node));
  fread(&node->freq, sizeof(int), 1, input);
  fread(&node->data, sizeof(long), 1, input);

  // read the left and right offsets from the input file
  long left_offset, right_offset;
  fread(&left_offset, sizeof(long), 1, input);
  fread(&right_offset, sizeof(long), 1, input);

  // read the left and right subtrees from the input file
  if (left_offset != -1) {
    fseek(input, left_offset, SEEK_SET);
    node->left = _huffman_read_huffman_table_helper(input);
  } else {
    node->left = NULL;
  }

  if (right_offset != -1) {
    fseek(input, right_offset, SEEK_SET);
    node->right = _huffman_read_huffman_table_helper(input);
  } else {
    node->right = NULL;
  }

  return node;
}

void _huffman_populate_tree_with_words(huffman_tree *tree, FILE *input) {
  // read the number of words from the input file
  unsigned int word_count;
  fread(&word_count, sizeof(unsigned int), 1, input);

  // recursively populate the tree with the words
  _huffman_populate_tree_with_words_helper(tree->root, input);
}

void _huffman_populate_tree_with_words_helper(huffman_node *node, FILE *input) {
  if (node == NULL) {
    return;
  }

  if (node->data == (char *)-1) {
    node->data = NULL;
    _huffman_populate_tree_with_words_helper(node->left, input);
    _huffman_populate_tree_with_words_helper(node->right, input);
  } else {
    unsigned int length;
    fseek(input, (long)node->data, SEEK_SET);
    fread(&length, sizeof(unsigned int), 1, input);
    node->data = malloc(length + 1);
    fread(node->data, sizeof(char), length, input);
    node->data[length] = '\0';
  }
}

void huffman_decode_file_helper(FILE *input, FILE *output, huffman_header *header, huffman_tree *tree) {
  // traverse the huffman tree to decode the input file
  huffman_node *current = tree->root;
  unsigned int word_count = 0;

  // printf("word_count: %d\n", header->word_count);
  while (!feof(input)) {
    // read the next byte from the input file
    unsigned char byte;
    fread(&byte, sizeof(unsigned char), 1, input);
    // printf("byte: 0x%x\n", byte);

    // traverse the huffman tree
    for (int i = 0; i < 8; i++) {
      // printf("bit: %d\n", (byte >> i) & 1);

      if ((byte >> i) & 1) {
        current = current->right;
      } else {
        current = current->left;
      }

      if (current == NULL) {
        printf("current is null\n");
        current = tree->root;
      } else if (current->data != NULL) {
        fprintf(output, "%s", current->data);
        // printf("current: %s\n", current->data);
        current = tree->root;
        word_count++;
      }

      if (word_count >= header->word_count) {
        break;
      }
    }
  }
}

void huffman_encode_file_per_word(char *input_file, char *output_file) {
  // create a word frequency table
  trie *word_freqs = _huffman_get_word_freq_table_from_file(input_file);

  // create a huffman tree from the word frequency table
  huffman_tree *tree = huffman_create_tree_from_word_freq_table(word_freqs);

  // printf("Huffman tree:\n");
  // huffman_print(tree);
  // printf("\n====================\n");

  // create code table from the huffman tree
  trie *code_table = _huffman_word_create_code_table(tree);

  // encode the input file using the word code table
  huffman_encode_file(input_file, output_file, tree, code_table);
}

trie *_huffman_get_word_freq_table_from_file(char *input_file) {
  trie *word_freq_table = trie_create();

  // open the input file for reading
  FILE *input = fopen(input_file, "r");

  // if the file does not exist, return NULL
  if (input == NULL) {
    return NULL;
  }

  // read the file line by line and update the word frequency table
  char line[LINE_BUFFER_SIZE];
  while (fgets(line, LINE_BUFFER_SIZE, input) != NULL) {
    int n = 0, offset = 0;
    char word_buffer[LINE_BUFFER_SIZE] = {0};
    char delim[2] = {0};

    // printf("line: %s\n", line);
    for (offset = 0; line[offset] != '\0' && offset < LINE_BUFFER_SIZE; offset += n) {
      n = 0;
      sscanf(line + offset, "%[^ \t\n]%n", word_buffer, &n);
      // printf("word: %s\n", word_buffer);

      if (strlen(word_buffer) > 0) {
        int steps = 0;
        int freq = (int)trie_search(word_freq_table, word_buffer, &steps, false);
        if (freq == 0) {
          trie_insert(word_freq_table, word_buffer, (void *)1);
        } else {
          trie_insert(word_freq_table, word_buffer, (void *)(freq + 1));
        }
        word_buffer[0] = '\0';
      }

      sscanf(line + offset + n, "%c", delim);
      n++;
      // printf("delim: '%s'\n", delim);

      if (strlen(delim) > 0) {
        int steps = 0;
        int freq = (int)trie_search(word_freq_table, delim, &steps, false);
        if (freq == 0) {
          trie_insert(word_freq_table, delim, (void *)1);
        } else {
          trie_insert(word_freq_table, delim, (void *)(freq + 1));
        }
        delim[0] = '\0';
      }
    }
  }

  // close the file
  fclose(input);

  // get the keys from the word frequency table
  return word_freq_table;
}

huffman_tree *huffman_create_tree_from_word_freq_table(trie *word_freqs) {
  // create a priority queue
  priority_queue *queue = priority_queue_create(NULL, _freq_compare);
  dynamic_array *words = trie_keys(word_freqs);
  int steps = 0;

  // insert the words into the priority queue
  for (int i = 0; i < words->size; i++) {
    huffman_node *node = malloc(sizeof(huffman_node));
    node->data = words->array[i];
    node->freq = (int)trie_search(word_freqs, node->data, &steps, false);
    node->left = NULL;
    node->right = NULL;
    priority_queue_insert(queue, node);
  }

  // build the huffman tree
  while (priority_queue_size(queue) > 1) {
    huffman_node *left = NULL, *right = NULL, *parent = NULL;

    // extract the two nodes with the lowest frequency
    priority_queue_extract(queue, (void **)&left);
    priority_queue_extract(queue, (void **)&right);

    // create a parent node
    parent = malloc(sizeof(huffman_node));
    parent->data = NULL;
    parent->freq = left->freq + right->freq;
    parent->left = left;
    parent->right = right;

    // insert the parent node back into the priority queue
    priority_queue_insert(queue, parent);
  }

  // extract the root node from the priority queue
  huffman_node *root;
  priority_queue_extract(queue, (void **)&root);

  // deallocate the priority queue
  priority_queue_destroy(queue);

  // create a huffman tree
  huffman_tree *tree = malloc(sizeof(huffman_tree));
  tree->root = root;

  return tree;
}

trie *_huffman_word_create_code_table(huffman_tree *tree) {
  // create a trie to store the word code table
  trie *code_table = trie_create();

  // bitvector to store the word code
  bitvector *code = bitvector_create(0);

  // traverse the huffman tree to generate the word code table
  _huffman_word_traverse_tree(tree->root, code, 0, code_table);

  // trie_print(code_table);

  return code_table;
}

void _huffman_word_traverse_tree(huffman_node *node, bitvector *code, int depth, trie *code_table) {
 if (node == NULL) {
    bitvector_destroy(code);
    return;
  }

  if (node->data != NULL) {
    // insert the word code into the code table
    bitvector *copy = bitvector_copy(code);
    trie_insert(code_table, node->data, copy);
  } else {
    // traverse the left subtree
    bitvector *copy_left = bitvector_copy(code);
    bitvector_append(copy_left, 0);
    _huffman_word_traverse_tree(node->left, copy_left, depth + 1, code_table);

    // traverse the right subtree
    bitvector *copy_right = bitvector_copy(code);
    bitvector_append(copy_right, 1);
    _huffman_word_traverse_tree(node->right, copy_right, depth + 1, code_table);
  }
}