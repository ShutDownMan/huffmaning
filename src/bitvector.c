#include <stdlib.h>
#include <stdio.h>

#include "bitvector.h"

bitvector* bitvector_create(int size) {
    bitvector* bv = malloc(sizeof(bitvector));
    bv->size = size;
    bv->capacity = size / 8 + 1;
    bv->bits = malloc(sizeof(char) * bv->capacity);
    return bv;
}

void bitvector_destroy(bitvector* vector) {
    free(vector->bits);
    free(vector);
}

char bitvector_get(const bitvector* vector, int index) {
    // return vector->bits[index];
    int byte_index = index / 8;
    int bit_index = index % 8;
    return (vector->bits[byte_index] >> bit_index) & 1;
}

void bitvector_set(bitvector* vector, int index, char value) {
    // vector->bits[index] = value;
    int byte_index = index / 8;
    int bit_index = index % 8;
    if (value) {
        vector->bits[byte_index] |= 1 << bit_index;
    } else {
        vector->bits[byte_index] &= ~(1 << bit_index);
    }
}

void bitvector_append(bitvector* vector, char value) {
    if (vector->size / 8 + 1 >= vector->capacity) {
        vector->capacity *= 2;
        vector->bits = realloc(vector->bits, sizeof(char) * vector->capacity);
    }
    bitvector_set(vector, vector->size, value);
    vector->size++;
}

bitvector *bitvector_from_byte(unsigned char byte) {
    bitvector* bv = bitvector_create(8);
    for (int i = 0; i < 8; i++) {
        bitvector_append(bv, (byte >> i) & 1);
    }
    return bv;
}

void bitvector_concat(bitvector* vector, const bitvector* other) {
    // TODO: Improve this horrible implementation
    if (other->size == 0) {
        return;
    }

    int byte_count = other->size / 8 + 1;
    for (int i = 0; i < byte_count; i++) {
        for (int j = 0; j < 8 && i * 8 + j < other->size; j++) {
            bitvector_append(vector, (other->bits[i] >> j) & 1);
        }
    }

}

bitvector *bitvector_copy(const bitvector* vector) {
    bitvector* bv = malloc(sizeof(bitvector));
    bv->size = vector->size;
    bv->bits = malloc(sizeof(char) * vector->size / 8 + 1);
    bv->capacity = vector->size / 8 + 1;
    // Copy the bits
    for (int i = 0; i < vector->size / 8 + 1; i++) {
        bv->bits[i] = vector->bits[i];
    }
    return bv;
}

// bitvector* bitvector_compress(const bitvector* vector) {
//     bitvector* bv = malloc(sizeof(bitvector));
//     bv->bits_size = vector->size;
//     bv->size = vector->size / 8 + 1;
//     bv->capacity = bv->size + 1;
//     bv->bits = malloc(sizeof(char) * bv->capacity);
//     // Initialize all bits to 0
//     for (int i = 0; i < bv->size; i++) {
//         bv->bits[i] = 0;
//     }
//     // Set the bits
//     for (int i = 0; i < vector->size; i++) {
//         if (vector->bits[i]) {
//             bv->bits[i / 8] |= 1 << (i % 8);
//         }
//     }
//     return bv;
// }

// bitvector* bitvector_decompress(const bitvector* vector) {
//     bitvector* bv = malloc(sizeof(bitvector));
//     bv->size = vector->size * 8;
//     bv->bits_size = -1;
//     bv->capacity = bv->size + 1;
//     bv->bits = malloc(sizeof(char) * bv->capacity);
//     // Initialize all bits to 0
//     for (int i = 0; i < bv->size; i++) {
//         bv->bits[i] = 0;
//     }
//     // Set the bits
//     for (int i = 0; i < vector->size; i++) {
//         for (int j = 0; j < 8; j++) {
//             if (vector->bits[i] & (1 << j)) {
//                 bv->bits[i * 8 + j] = 1;
//             }
//         }
//     }
//     return bv;
// }

void bitvector_reset(bitvector* vector) {
    for (int i = 0; i < vector->size / 8 + 1; i++) {
        vector->bits[i] = 0;
    }
    vector->size = 0;
}

int bitvector_size(const bitvector* vector) {
    return vector->size;
}

void bitvector_print(bitvector* bv) {
    int byte_count = bv->size / 8 + 1;

    // printf("byte_count: %d\n", byte_count);
    // printf("size: %d\n", bv->size);
    for (int i = 0; i < byte_count; i++) {
        for (int j = 0; j < 8 && i * 8 + j < bv->size; j++) {
            printf("%d", (bv->bits[i] >> j) & 1);
        }
        printf(" ");
    }


    printf("\n");
}