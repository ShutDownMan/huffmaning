#ifndef BITVECTOR_H
#define BITVECTOR_H

typedef struct bitvector {
    // Array of bits
    unsigned char* bits;
    // Number of bits
    int size;
    // Capacity of the array
    unsigned int capacity;
} bitvector;

bitvector* bitvector_create(int size);

void bitvector_destroy(bitvector* vector);

char bitvector_get(const bitvector* vector, int index);

void bitvector_set(bitvector* vector, int index, char value);

void bitvector_append(bitvector* vector, char value);

bitvector *bitvector_from_byte(unsigned char byte);

void bitvector_concat(bitvector* vector, const bitvector* other);

bitvector *bitvector_copy(const bitvector* vector);

// bitvector* bitvector_compress(const bitvector* vector);

// bitvector* bitvector_decompress(const bitvector* vector);

void bitvector_reset(bitvector* vector);

int bitvector_size(const bitvector* vector);

void bitvector_print(bitvector* bv);

#endif // BITVECTOR_H