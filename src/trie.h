#ifndef TRIE_H
#define TRIE_H

#include <stdbool.h>
#include "bitvector.h"
#include "dynamic_array.h"

typedef struct trie_node {
    struct trie_node *children[256];
    void *data;
} trie_node;

typedef struct trie {
    trie_node *root;
} trie;

/**
 * Creates a new trie.
 * @return The new trie.
 */
trie* trie_create();

/**
 * Destroys the trie.
 * @param t The trie to destroy.
 * @param destroy_data The function to destroy the data in the trie.
 */
void trie_destroy(trie* t, void (*destroy_data)(void* data));

/**
 * Helper function to destroy the trie.
 * @param node The current node.
 * @param destroy_data The function to destroy the data in the trie.
 */
void _trie_destroy_helper(trie_node* node, void (*destroy_data)(void* data));

/**
 * Inserts a word into the trie.
 * @param t The trie.
 * @param word The word to insert.
 * @return true if successful, false if an error occurs.
 * @param data The data to associate with the word.
 */
bool trie_insert(trie* t, const char* word, void* data);

void trie_print(trie* t);

void _trie_print_helper(trie_node* node, int level);

/**
 * Returns the words in the trie.
 * @param t The trie.
 * @return The words in the trie.
 */
dynamic_array *trie_keys(const trie* t);

/**
 * Helper function to get the keys in the trie.
 * @param node The current node.
 * @param prefix The current prefix.
 * @param keys The array of keys.
 * @param index The current index.
 */
void _trie_keys_helper(const trie_node* node, const char* prefix, dynamic_array *keys, int index);

/**
 * Searches for a word in the trie.
 * @param t The trie.
 * @param word The word to search for.
 * @param greedy If true, return the data associated with the first prefix of the word found.
 * @return The data associated with the word, or NULL if the word is not found.
 */
void* trie_search(const trie* t, const char* word, int *steps, bool greedy);

/**
 * Removes a word from the trie.
 * @param t The trie.
 * @param word The word to remove.
 * @param destroy_data The function to destroy the data in the trie.
 * @return true if successful, false if an error occurs.
 */
bool trie_remove(trie* t, const char* word, void (*destroy_data)(void* data));

#endif // TRIE_H