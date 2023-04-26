#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <stdbool.h>

typedef struct priority_queue {
    void** heap;
    int size;
    int capacity;
    void (*destroy)(void* data);
    int (*compare)(const void* key1, const void* key2);
} priority_queue;

priority_queue* priority_queue_create(void (*destroy)(void* data),
                                       int (*compare)(const void* key1, const void* key2));

void priority_queue_destroy(priority_queue* queue);

int priority_queue_insert(priority_queue* queue, const void* data);

int priority_queue_extract(priority_queue* queue, void** data);

void* priority_queue_peek(const priority_queue* queue);

int priority_queue_size(const priority_queue* queue);

bool priority_queue_is_empty(const priority_queue* queue);

#endif
