
#include "trie.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

trie* trie_create() {
    trie* t = malloc(sizeof(trie));
    t->root = malloc(sizeof(trie_node));
    for (int i = 0; i < 256; i++) {
        t->root->children[i] = NULL;
    }
    t->root->data = NULL;
    return t;
}

void trie_destroy(trie* t, void (*destroy_data)(void* data)) {
    if (t == NULL) {
        return;
    }
    _trie_destroy_helper(t->root, destroy_data);
    free(t->root);
    free(t);
}

void _trie_destroy_helper(trie_node* node, void (*destroy_data)(void* data)) {
    if (node == NULL) {
        return;
    }
    for (int i = 0; i < 256; i++) {
        _trie_destroy_helper(node->children[i], destroy_data);
    }
    if (node->data != NULL && destroy_data != NULL) {
        destroy_data(node->data);
    }
    free(node);
}

bool trie_insert(trie* t, const char* word, void* data) {
    trie_node* current = t->root;
    for (int i = 0; word[i] != '\0'; i++) {
        if (current->children[(unsigned char)word[i]] == NULL) {
            current->children[(unsigned char)word[i]] = malloc(sizeof(trie_node));
            for (int j = 0; j < 256; j++) {
                current->children[(unsigned char)word[i]]->children[j] = NULL;
            }
            current->children[(unsigned char)word[i]]->data = NULL;
        }
        current = current->children[(unsigned char)word[i]];
    }
    current->data = data;
    return true;
}

void trie_print(trie* t) {
    _trie_print_helper(t->root, 0);
}

void _trie_print_helper(trie_node* node, int level) {
    if (node == NULL) {
        return;
    }
    for (int i = 0; i < 256; i++) {
        if (node->children[i] != NULL) {
            for (int j = 0; j < level; j++) {
                printf("  ");
            }
            printf("%c ", (char)i);
            bitvector_print(node->children[i]->data);
            _trie_print_helper(node->children[i], level + 1);
        }
    }
}

dynamic_array *trie_keys(const trie* t) {
    dynamic_array *keys = dynamic_array_create();
    _trie_keys_helper(t->root, "", keys, 0);
    return keys;
}

void _trie_keys_helper(const trie_node* node, const char* prefix, dynamic_array *keys, int index) {
    if (node == NULL) {
        return;
    }
    if (node->data != NULL) {
        char *key = malloc((strlen(prefix) + 1) * sizeof(char));
        strcpy(key, prefix);
        dynamic_array_insert(keys, key);
    }
    for (int i = 0; i < 256; i++) {
        char *new_prefix = malloc((strlen(prefix) + 2) * sizeof(char));
        strcpy(new_prefix, prefix);
        new_prefix[strlen(prefix)] = (char)i;
        new_prefix[strlen(prefix) + 1] = '\0';
        _trie_keys_helper(node->children[i], new_prefix, keys, index + 1);
    }
}

void* trie_search(const trie* t, const char* word, int *steps, bool greedy) {
    trie_node* current = t->root;
    void *last_data = NULL;
    int last_data_steps = 0;
    for (int i = 0; word[i] != '\0'; i++) {
        if (current->data != NULL) {
            if (greedy) {
                return current->data;
            } else {
                last_data = current->data;
            }
        }
        if (current->children[(unsigned char)word[i]] == NULL) {
            *steps = last_data_steps;
            return last_data;
        }
        *steps += 1;
        last_data_steps += 1;
        current = current->children[(unsigned char)word[i]];
    }

    return current->data;
}

bool trie_remove(trie* t, const char* word, void (*destroy_data)(void* data)) {
    trie_node* current = t->root;
    for (int i = 0; word[i] != '\0'; i++) {
        if (current->children[(unsigned char)word[i]] == NULL) {
            return false;
        }
        current = current->children[(unsigned char)word[i]];
    }
    if (current->data != NULL && destroy_data != NULL) {
        destroy_data(current->data);
    }
    current->data = NULL;
    return true;
}
