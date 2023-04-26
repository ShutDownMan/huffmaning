#include <stdlib.h>
#include <stdio.h>

#include "bitvector.h"

bitvector* bitvector_create(int size) {
    bitvector* bv = malloc(sizeof(bitvector));
    bv->size = size;
    bv->capacity = size + 1;
    bv->bits = malloc(sizeof(char) * bv->capacity);
    return bv;
}

void bitvector_destroy(bitvector* vector) {
    free(vector->bits);
    free(vector);
}

char bitvector_get(const bitvector* vector, int index) {
    return vector->bits[index];
}

void bitvector_set(bitvector* vector, int index, char value) {
    vector->bits[index] = value;
}

void bitvector_append(bitvector* vector, char value) {
    if (vector->size == vector->capacity) {
        vector->capacity *= 2;
        vector->bits = realloc(vector->bits, sizeof(char) * vector->capacity);
    }
    vector->bits[vector->size] = value;
    vector->size++;
}

bitvector *bitvector_copy(const bitvector* vector) {
    bitvector* bv = malloc(sizeof(bitvector));
    bv->size = vector->size;
    bv->bits = malloc(sizeof(char) * vector->size);
    bv->capacity = vector->size + 1;
    // Copy the bits
    for (int i = 0; i < vector->size; i++) {
        bv->bits[i] = vector->bits[i];
    }
    return bv;
}

bitvector* bitvector_compress(const bitvector* vector) {
    bitvector* bv = malloc(sizeof(bitvector));
    bv->size = vector->size / 8 + 1;
    bv->capacity = bv->size + 1;
    bv->bits = malloc(sizeof(char) * bv->capacity);
    // Initialize all bits to 0
    for (int i = 0; i < bv->size; i++) {
        bv->bits[i] = 0;
    }
    // Set the bits
    for (int i = 0; i < vector->size; i++) {
        if (vector->bits[i]) {
            bv->bits[i / 8] |= 1 << (i % 8);
        }
    }
    return bv;
}

bitvector* bitvector_decompress(const bitvector* vector) {
    bitvector* bv = malloc(sizeof(bitvector));
    bv->size = vector->size * 8;
    bv->capacity = bv->size + 1;
    bv->bits = malloc(sizeof(char) * bv->capacity);
    // Initialize all bits to 0
    for (int i = 0; i < bv->size; i++) {
        bv->bits[i] = 0;
    }
    // Set the bits
    for (int i = 0; i < vector->size; i++) {
        for (int j = 0; j < 8; j++) {
            if (vector->bits[i] & (1 << j)) {
                bv->bits[i * 8 + j] = 1;
            }
        }
    }
    return bv;
}

int bitvector_size(const bitvector* vector) {
    return vector->size;
}

void bitvector_print(bitvector* bv) {
    for (int i = 0; i < bv->size; i++) {
        printf("%d", bv->bits[i]);
    }
    printf("\n");
}