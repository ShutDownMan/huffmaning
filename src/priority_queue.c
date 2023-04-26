#include <stdlib.h>
#include <string.h>

#include "priority_queue.h"

#define PARENT(i) ((i - 1) / 2)
#define LEFT_CHILD(i) (2 * i + 1)
#define RIGHT_CHILD(i) (2 * i + 2)

priority_queue* priority_queue_create(void (*destroy)(void* data), int (*compare)(const void* key1, const void* key2)) {
    priority_queue* queue = malloc(sizeof(priority_queue));
    if (queue == NULL) {
        return NULL;
    }
    queue->heap = NULL;
    queue->size = 0;
    queue->capacity = 0;
    queue->destroy = destroy;
    queue->compare = compare;
    return queue;
}

void priority_queue_destroy(priority_queue* queue) {
    if (queue == NULL) {
        return;
    }
    if (queue->destroy != NULL) {
        for (int i = 0; i < queue->size; i++) {
            queue->destroy(queue->heap[i]);
        }
    }
    free(queue->heap);
    free(queue);
}

int priority_queue_insert(priority_queue* queue, const void* data) {
    void* temp;
    int index, parent;
    if (queue == NULL) {
        return -1;
    }
    if (queue->size == queue->capacity) {
        int new_capacity = queue->capacity == 0 ? 1 : queue->capacity * 2;
        temp = realloc(queue->heap, new_capacity * sizeof(void*));
        if (temp == NULL) {
            return -1;
        }
        queue->heap = temp;
        queue->capacity = new_capacity;
    }
    queue->heap[queue->size] = (void*)data;
    index = queue->size;
    parent = PARENT(index);
    while (index > 0 && queue->compare(queue->heap[index], queue->heap[parent]) > 0) {
        temp = queue->heap[index];
        queue->heap[index] = queue->heap[parent];
        queue->heap[parent] = temp;
        index = parent;
        parent = PARENT(index);
    }
    queue->size++;
    return 0;
}

int priority_queue_extract(priority_queue* queue, void** data) {
    void* temp;
    int index, left_child, right_child, largest;
    if (queue == NULL || queue->size == 0) {
        return -1;
    }
    *data = queue->heap[0];
    queue->heap[0] = queue->heap[queue->size - 1];
    queue->size--;
    index = 0;
    while (true) {
        left_child = LEFT_CHILD(index);
        right_child = RIGHT_CHILD(index);
        largest = index;
        if (left_child < queue->size && queue->compare(queue->heap[left_child], queue->heap[largest]) > 0) {
            largest = left_child;
        }
        if (right_child < queue->size && queue->compare(queue->heap[right_child], queue->heap[largest]) > 0) {
            largest = right_child;
        }
        if (largest == index) {
            break;
        }
        temp = queue->heap[index];
        queue->heap[index] = queue->heap[largest];
        queue->heap[largest] = temp;
        index = largest;
    }
    if (queue->size > 0 && queue->size == queue->capacity / 4) {
        int new_capacity = queue->capacity / 2;
        temp = realloc(queue->heap, new_capacity * sizeof(void*));
        if (temp == NULL) {
            return -1;
        }
        queue->heap = temp;
        queue->capacity = new_capacity;
    }
    return 0;
}

void* priority_queue_peek(const priority_queue* queue) {
    if (queue == NULL || queue->size == 0) {
        return NULL;
    }
    return queue->heap[0];
}

int priority_queue_size(const priority_queue* queue) {
    if (queue == NULL) {
        return -1;
    }
    return queue->size;
}

bool priority_queue_is_empty(const priority_queue* queue) {
    if (queue == NULL) {
        return false;
    }
    return queue->size == 0;
}