#ifndef BITVECTOR_H
#define BITVECTOR_H

typedef struct bitvector {
    unsigned char* bits;
    int size;
    int bits_size;
    unsigned int capacity;
} bitvector;

bitvector* bitvector_create(int size);

void bitvector_destroy(bitvector* vector);

char bitvector_get(const bitvector* vector, int index);

void bitvector_set(bitvector* vector, int index, char value);

void bitvector_append(bitvector* vector, char value);

bitvector *bitvector_copy(const bitvector* vector);

bitvector* bitvector_compress(const bitvector* vector);

bitvector* bitvector_decompress(const bitvector* vector);

int bitvector_size(const bitvector* vector);

void bitvector_print(bitvector* bv);

#endif // BITVECTOR_H