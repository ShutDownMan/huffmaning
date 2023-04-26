#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bitvector.h"
#include "huffman.h"
#include "priority_queue.h"

void huffman_node_destroy(huffman_node *tree) {
  if (tree == NULL) {
    return;
  }
  huffman_node_destroy(tree->left);
  huffman_node_destroy(tree->right);
  free(tree);
}

int huffman_node_compare(const huffman_node *node1, const huffman_node *node2) {
  if (node1 == NULL || node2 == NULL) {
    return 0;
  }
  return node2->freq - node1->freq;
}

// Function to create a huffman tree from a character frequency table
huffman_tree *huffman_create_tree_from_char_freq_table(int *char_freq_table) {
  priority_queue *queue = priority_queue_create(
      (void (*)(void *))huffman_node_destroy,
      (int (*)(const void *, const void *))huffman_node_compare);
  if (queue == NULL) {
    return NULL;
  }
  for (int i = 0; i < 256; i++) {
    if (char_freq_table[i] > 0) {
      huffman_node *node = malloc(sizeof(huffman_node));
      if (node == NULL) {
        priority_queue_destroy(queue);
        return NULL;
      }
      node->left = NULL;
      node->right = NULL;
      node->freq = char_freq_table[i];
      node->c = i;
      if (priority_queue_insert(queue, node) != 0) {
        priority_queue_destroy(queue);
        return NULL;
      }
    }
  }
  while (queue->size > 1) {
    huffman_node *node1, *node2, *node;
    if (priority_queue_extract(queue, (void **)&node1) != 0) {
      priority_queue_destroy(queue);
      return NULL;
    }
    if (priority_queue_extract(queue, (void **)&node2) != 0) {
      priority_queue_destroy(queue);
      return NULL;
    }
    node = malloc(sizeof(huffman_node));
    if (node == NULL) {
      priority_queue_destroy(queue);
      return NULL;
    }
    node->left = node1;
    node->right = node2;
    node->freq = node1->freq + node2->freq;
    if (priority_queue_insert(queue, node) != 0) {
      priority_queue_destroy(queue);
      return NULL;
    }
  }
  huffman_tree *tree = malloc(sizeof(huffman_tree));
  if (tree == NULL) {
    priority_queue_destroy(queue);
    return NULL;
  }
  if (priority_queue_extract(queue, (void **)&tree->root) != 0) {
    priority_queue_destroy(queue);
    return NULL;
  }
  priority_queue_destroy(queue);
  return tree;
}

// Function to delete a huffman tree
void huffman_delete_tree(huffman_tree *tree) {
  if (tree == NULL) {
    return;
  }
  huffman_node_destroy(tree->root);
  free(tree);
}

// Function to print a huffman tree
void huffman_print_tree(huffman_node *tree, int level) {
  if (tree == NULL) {
    return;
  }
  for (int i = 0; i < level; i++) {
    printf("  ");
  }
  if (tree->left == NULL && tree->right == NULL) {
    // check if word is null
    if (tree->word != NULL) {
      printf("%s (%d)\n", tree->word, tree->freq);
    } else {
      printf("%c (%d)\n", tree->c, tree->freq);
    }
  } else {
    printf("%d\n", tree->freq);
  }
  huffman_print_tree(tree->left, level + 1);
  huffman_print_tree(tree->right, level + 1);
}

// Function to populate a character code table from a huffman tree
void huffman_create_char_code_table(huffman_node *tree, bitvector **code_table,
                                    bitvector *code) {
  // Create a bitvector for the code
  bitvector *right_code = bitvector_copy(code);
  bitvector *left_code = bitvector_copy(code);
  if (left_code == NULL) {
    return;
  }

  // If the node is a leaf, add the code to the code table
  if (tree->left == NULL && tree->right == NULL) {
    code_table[tree->c] = left_code;
    return;
  }

  // If the node has a right child, add a 1 to the code and recurse
  if (tree->right != NULL) {
    bitvector_append(right_code, 1);
    huffman_create_char_code_table(tree->right, code_table, right_code);
  }

  // If the node has a left child, add a 0 to the code and recurse
  if (tree->left != NULL) {
    bitvector_append(left_code, 0);
    huffman_create_char_code_table(tree->left, code_table, left_code);
  }

  // Free the code copy
  bitvector_destroy(left_code);
}

// Function to compress a file using huffman encoding per character
void huffman_encode_file_per_char(char *input_file, char *output_file) {
  // Create a character frequency table
  int char_freq_table[256] = {0};
  FILE *file = fopen(input_file, "r");
  if (file == NULL) {
    return;
  }

  // Read the file line by line and update the character frequency table
  char line[256];
  while (fgets(line, 256, file) != NULL) {
    for (int i = 0; i < strlen(line); i++) {
      char_freq_table[(int)line[i]]++;
    }
  }
  fclose(file);

  // Create a huffman tree from the character frequency table
  huffman_tree *tree =
      huffman_create_tree_from_char_freq_table(char_freq_table);
  if (tree == NULL) {
    return;
  }
  printf("Huffman tree:\n");
  huffman_print_tree(tree->root, 0);

  // Create a character code table from the huffman tree
  bitvector *char_code_table[256];
  for (int i = 0; i < 256; i++) {
    char_code_table[i] = bitvector_create(0);
  }
  huffman_create_char_code_table(tree->root, char_code_table,
                                 bitvector_create(0));

  // Open the input file for reading
  file = fopen(input_file, "r");
  if (file == NULL) {
    return;
  }

  bitvector *output_vec = bitvector_create(0);
  // Read the file line by line and write the encoded characters to the output
  // vec bit by bit
  while (fgets(line, 256, file) != NULL) {
    for (int i = 0; i < strlen(line); i++) {
      bitvector *code = char_code_table[(int)line[i]];
      for (int j = 0; j < code->size; j++) {
        bitvector_append(output_vec, code->bits[j]);
      }
    }
  }

  fclose(file);

  // Open the output file for writing
  FILE *output = fopen(output_file, "wb");
  if (output == NULL) {
    return;
  }

  // write tree to output file
  huffman_write_char_tree(tree, output);

  // Write the output bitvector to the output file bit by bit
  bitvector *compressed_output_vec = bitvector_compress(output_vec);
  fwrite(compressed_output_vec->bits, sizeof(char), compressed_output_vec->size,
         output);
  fclose(output);

  // Print the compression ratio
  // printf("Compression ratio: %f\n", (float) file_size(output_file) /
  // file_size(input_file));

  // Print the compression time
  // printf("Compression time: %f\n", (float) clock() / CLOCKS_PER_SEC);

  // Print the compression speed
  // printf("Compression speed: %f\n", (float) file_size(input_file) / clock() *
  // CLOCKS_PER_SEC);

  // Print the entropy
  // printf("Entropy: %f\n", entropy(char_freq_table));

  // Print the average code length
  // printf("Average code length: %f\n", average_code_length(char_freq_table,
  // char_code_table));

  // Print the code table
  printf("Code table:\n");
  for (int i = 0; i < 256; i++) {
    if (char_code_table[i] != NULL && char_code_table[i]->size > 0) {
      printf("%c: ", i);
      bitvector_print(char_code_table[i]);
    }
  }

  // Print the tree
  // printf("Tree:\n");
  // huffman_print_tree(tree->root, 0);

  // Delete the huffman tree
  huffman_delete_tree(tree);

  // Delete the character code table
  for (int i = 0; i < 256; i++) {
    if (char_code_table[i] != NULL) {
      bitvector_destroy(char_code_table[i]);
    }
  }
}

// huffman_write_char_tree
void huffman_write_char_tree(huffman_tree *tree, FILE *output) {
  // Write the tree to the output file
  huffman_write_char_tree_helper(tree->root, output);
}

// huffman_write_char_tree_helper
void huffman_write_char_tree_helper(huffman_node *node, FILE *output) {
  // If the node is a leaf, write a 1 and the character to the output file
  if (node->left == NULL && node->right == NULL) {
    printf("1%c", node->c);
    fwrite("1", sizeof(char), 1, output);
    fwrite(&node->c, sizeof(char), 1, output);
    return;
  }

  printf("0");
  // If the node is not a leaf, write a 0 to the output file
  fwrite("0", sizeof(char), 1, output);

  // Recurse on the left and right children
  huffman_write_char_tree_helper(node->left, output);
  huffman_write_char_tree_helper(node->right, output);
}

// huffman_decode_file_helper
void huffman_decode_char_file_helper(bitvector *input_bits, huffman_tree *tree,
                                     FILE *output) {
  // Start at the root of the tree
  huffman_node *node = tree->root;

  // Loop through the input bits
  for (int i = 0; i < input_bits->size; i++) {
    // If the bit is 0, go left
    if (input_bits->bits[i] == 0) {
      node = node->left;
    }
    // If the bit is 1, go right
    else {
      node = node->right;
    }

    // If the node is a leaf, write the character to the output file
    if (node->left == NULL && node->right == NULL) {
      fwrite(&node->c, sizeof(char), 1, output);
      node = tree->root;
    }
  }
}

// Function to decompress a file using huffman decoding per character
void huffman_decode_file_per_char(char *input_file, char *output_file) {
  // Open the input file for reading
  FILE *input = fopen(input_file, "rb");
  if (input == NULL) {
    return;
  }

  // Read the huffman tree from the input file
  huffman_tree *tree = huffman_read_char_tree(input);
  if (tree == NULL) {
    return;
  }
  // print the tree
  huffman_print_tree(tree->root, 0);

  // Open the output file for writing
  FILE *output = fopen(output_file, "w");
  if (output == NULL) {
    return;
  }

  // create a bitvector to store the input bits
  bitvector *input_bits = bitvector_create(0);

  // fill the bitvector with the input bits
  char bit;
  while (fread(&bit, sizeof(char), 1, input) == 1) {
    bitvector_append(input_bits, bit);
  }

  // decompress bitvector
  bitvector *decompressed_input_bits = bitvector_decompress(input_bits);

  // Read the input file bit by bit and decode the characters
  huffman_decode_char_file_helper(decompressed_input_bits, tree, output);

  fclose(input);
  fclose(output);
}

// huffman_read_tree
huffman_tree *huffman_read_char_tree(FILE *input) {
  // Read the tree from the input file
  huffman_node *root = huffman_read_char_tree_helper(input);
  if (root == NULL) {
    return NULL;
  }

  // Create a huffman tree from the root node
  huffman_tree *tree = malloc(sizeof(huffman_tree));
  tree->root = root;
  return tree;
}

// huffman_read_tree_helper
huffman_node *huffman_read_char_tree_helper(FILE *input) {
  // Read a bit from the input file
  char bit;
  fread(&bit, sizeof(char), 1, input);

  // If the bit is 1, read the next byte as the character and create a leaf node
  if (bit == '1') {
    char c;
    fread(&c, sizeof(char), 1, input);
    return huffman_create_node(c, 0, NULL, NULL);
  }

  // If the bit is 0, recurse on the left and right children
  huffman_node *left = huffman_read_char_tree_helper(input);
  huffman_node *right = huffman_read_char_tree_helper(input);

  // Create a node with the left and right children
  return huffman_create_node(0, 0, left, right);
}

// huffman_create_node
huffman_node *huffman_create_node(char c, int freq, huffman_node *left,
                                  huffman_node *right) {
  // Create a new node
  huffman_node *node = malloc(sizeof(huffman_node));
  node->c = c;
  node->freq = freq;
  node->left = left;
  node->right = right;
  return node;
}

// Function to create a huffman tree from a word frequency table
huffman_tree *huffman_create_tree_from_word_freq_table(char **words,
                                                       unsigned int word_count,
                                                       int *word_freq_table) {
  priority_queue *queue = priority_queue_create(
      (void (*)(void *))huffman_node_destroy,
      (int (*)(const void *, const void *))huffman_node_compare);
  if (queue == NULL) {
    return NULL;
  }
  for (int i = 0; i < word_count; i++) {
    if (word_freq_table[i] > 0) {
      huffman_node *node = malloc(sizeof(huffman_node));
      if (node == NULL) {
        priority_queue_destroy(queue);
        return NULL;
      }
      node->left = NULL;
      node->right = NULL;
      node->freq = word_freq_table[i];
      node->word = words[i];
      if (priority_queue_insert(queue, node) != 0) {
        priority_queue_destroy(queue);
        return NULL;
      }
    }
  }
  while (queue->size > 1) {
    huffman_node *node1, *node2, *node;
    if (priority_queue_extract(queue, (void **)&node1) != 0) {
      priority_queue_destroy(queue);
      return NULL;
    }
    if (priority_queue_extract(queue, (void **)&node2) != 0) {
      priority_queue_destroy(queue);
      return NULL;
    }
    node = malloc(sizeof(huffman_node));
    if (node == NULL) {
      priority_queue_destroy(queue);
      return NULL;
    }
    node->left = node1;
    node->right = node2;
    node->freq = node1->freq + node2->freq;
    if (priority_queue_insert(queue, node) != 0) {
      priority_queue_destroy(queue);
      return NULL;
    }
  }
  huffman_tree *tree = malloc(sizeof(huffman_tree));
  if (tree == NULL) {
    priority_queue_destroy(queue);
    return NULL;
  }
  if (priority_queue_extract(queue, (void **)&tree->root) != 0) {
    priority_queue_destroy(queue);
    return NULL;
  }
  priority_queue_destroy(queue);
  return tree;
}

// huffman_create_word_code_table
void huffman_create_word_code_table(huffman_node *tree, bitvector **code_table,
                                    bitvector *code, char **words,
                                    int word_count) {
  // Create a bitvector for the code
  bitvector *right_code = bitvector_copy(code);
  bitvector *left_code = bitvector_copy(code);

  // If the node is a leaf, add the code to the code table
  if (tree->left == NULL && tree->right == NULL) {
    // find the index of the word in the word table
    int index = -1;
    for (int i = 0; i < word_count; i++) {
      if (strcmp(words[i], tree->word) == 0) {
        index = i;
        break;
      }
    }

    if (index == -1) {
      printf("Error: word not found in word table\n");
      return;
    }

    printf("word: %s, index: %d\n", tree->word, index);

    // add the code to the code table
    code_table[index] = code;

    return;
  }

  // If the node has a right child, add a 1 to the code and recurse
  if (tree->right != NULL) {
    bitvector_append(right_code, 1);
    huffman_create_word_code_table(tree->right, code_table, right_code, words,
                                   word_count);
  }

  // If the node has a left child, add a 0 to the code and recurse
  if (tree->left != NULL) {
    bitvector_append(left_code, 0);
    huffman_create_word_code_table(tree->left, code_table, left_code, words,
                                   word_count);
  }

  // Free the code copy
  bitvector_destroy(left_code);
}

// Function to compress a file using huffman encoding per word
void huffman_encode_file_per_word(char *input_file, char *output_file) {
  // Open the input file for reading
  FILE *input = fopen(input_file, "r");
  if (input == NULL) {
    return;
  }

  // Read the input file and create a word frequency table
  char **words = malloc(sizeof(char *));
  if (words == NULL) {
    return;
  }

  unsigned int word_count = 0;
  int *word_freq_table = malloc(sizeof(int));
  if (word_freq_table == NULL) {
    return;
  }

  int word_freq_table_size = 1;
  char *word = malloc(sizeof(char));
  if (word == NULL) {
    return;
  }

  int word_size = 1;
  char c;
  while (fread(&c, sizeof(char), 1, input) == 1) {
    if (c == ' ' || c == '\n') {
      if (word_size > 1) {
        word[word_size - 1] = '\0';
        int index = -1;
        for (int i = 0; i < word_count; i++) {
          if (strcmp(words[i], word) == 0) {
            index = i;
            break;
          }
        }
        if (index == -1) {
          if (word_count == word_freq_table_size) {
            word_freq_table_size *= 2;
            word_freq_table =
                realloc(word_freq_table, word_freq_table_size * sizeof(int));
            if (word_freq_table == NULL) {
              return;
            }
          }
          words = realloc(words, (word_count + 1) * sizeof(char *));
          if (words == NULL) {
            return;
          }
          words[word_count] = malloc(word_size * sizeof(char));
          if (words[word_count] == NULL) {
            return;
          }
          strcpy(words[word_count], word);
          word_freq_table[word_count] = 1;
          word_count++;
        } else {
          word_freq_table[index]++;
        }
      }
      word = malloc(sizeof(char));
      if (word == NULL) {
        return;
      }
      word_size = 1;
    } else {
      word[word_size - 1] = c;
      word_size++;
      word = realloc(word, word_size * sizeof(char));
      if (word == NULL) {
        return;
      }
    }
  }

  // Create a huffman tree from the word frequency table
  huffman_tree *tree = huffman_create_tree_from_word_freq_table(
      words, word_count, word_freq_table);
  if (tree == NULL) {
    return;
  }
  // print the tree
  huffman_print_tree(tree->root, 0);

  // create encoding table
  bitvector **code_table = malloc(word_count * sizeof(bitvector *));
  for (int i = 0; i < word_count; i++) {
    code_table[i] = NULL;
  }

  // encode the words
  huffman_create_word_code_table(tree->root, code_table, bitvector_create(0),
                                 words, word_count);
  printf("Encoding table:\n");
  for (int i = 0; i < word_count; i++) {
    printf("%s: ", words[i]);
    if (code_table[i] == NULL) {
      // print the word
      printf("NULL TABLE ENTRY: %s\n", words[i]);
    } else {
      bitvector_print(code_table[i]);
    }
  }

  // compress all bitvectors
  for (int i = 0; i < word_count; i++) {
    bitvector_compress(code_table[i]);
  }

  // Open the output file for writing
  FILE *output = fopen(output_file, "wb");
  if (output == NULL) {
    return;
  }

  // Write the tree to the output file
  huffman_write_word_tree(tree, output);

  // Write the tree words to the output file
  for (int i = 0; i < word_count; i++) {
    fwrite(words[i], sizeof(char), strlen(words[i]) + 1, output);
  }

  // Write the encoded words to the output file
  fseek(input, 0, SEEK_SET);
  word = malloc(sizeof(char));
  if (word == NULL) {
    return;
  }

  word_size = 1;
  while (fread(&c, sizeof(char), 1, input) == 1) {
    if (c == ' ' || c == '\n') {
      if (word_size > 1) {
        word[word_size - 1] = '\0';
        int index = -1;
        for (int i = 0; i < word_count; i++) {
          if (strcmp(words[i], word) == 0) {
            index = i;
            break;
          }
        }
        if (index != -1) {
          // printf("%s - %d\n", word, code_table[index]->size);
          fwrite(code_table[index]->bits, sizeof(char), code_table[index]->size,
                 output);
        } else {
          printf("Error: word not found in code table\n");
        }
      }
      word = malloc(sizeof(char));
      if (word == NULL) {
        return;
      }
      word_size = 1;
    } else {
      word[word_size - 1] = c;
      word_size++;
      word = realloc(word, word_size * sizeof(char));
      if (word == NULL) {
        return;
      }
    }
  }
}

// huffman_write_word_tree
void huffman_write_word_tree(huffman_tree *tree, FILE *output) {
  int index = 0;
  // Write the tree to the output file
  huffman_write_word_tree_helper(tree->root, output, &index);
}

// huffman_write_word_tree_helper
void huffman_write_word_tree_helper(huffman_node *node, FILE *output,
                                    int *index) {
  // If the node is a leaf, write a 1 and the character to the output file
  if (node->left == NULL && node->right == NULL) {
    printf("1%c", *index);
    fwrite("1", sizeof(char), 1, output);
    fwrite(index, sizeof(int), 1, output);
    *index = *index + 1;
    return;
  }

  printf("0");
  // If the node is not a leaf, write a 0 to the output file
  fwrite("0", sizeof(char), 1, output);

  // Recurse on the left and right children
  huffman_write_word_tree_helper(node->left, output, index);
  huffman_write_word_tree_helper(node->right, output, index);
}
