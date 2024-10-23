#include <stdlib.h>
#include <string.h>

#include "priority_queue.h"

#define PARENT_AT(i) ((i - 1) / 2)
#define LEFT_CHILD_OF(i) (2 * i + 1)
#define RIGHT_CHILD_OF(i) (2 * i + 2)

priority_queue* priority_queue_create(void (*destroy)(void* data), int (*compare)(const void* key1, const void* key2)) {
    // allocate memory for the queue
    priority_queue* queue = malloc(sizeof(priority_queue));

    // if allocation fails, return NULL
    if (queue == NULL) {
        return NULL;
    }

    // initialize the queue
    queue->heap = NULL;
    queue->size = 0;
    queue->capacity = 0;
    queue->destroy = destroy;
    queue->compare = compare;

    return queue;
}

void priority_queue_destroy(priority_queue* queue) {
    // if queue does not exist, do nothing
    if (queue == NULL) {
        return;
    }

    // if destroy function exists
    if (queue->destroy != NULL) {
        // loop through the queue
        for (int i = 0; i < queue->size; i++) {
            // destroy the data at the index
            queue->destroy(queue->heap[i]);
        }
    }

    // free the memory
    free(queue->heap);
    free(queue);
}

int priority_queue_insert(priority_queue* queue, const void* data) {
    void* temp;
    int index, parent;

    // if queue does not exist
    if (queue == NULL) {
        return -1;
    }

    // if the queue is at capacity
    if (queue->size == queue->capacity) {
        // calculate new capacity
        int new_capacity = queue->capacity == 0 ? 1 : queue->capacity * 2;
        // reallocate memory
        temp = realloc(queue->heap, new_capacity * sizeof(void*));

        // if reallocation fails
        if (temp == NULL) {
            return -1;
        }

        // update the queue
        queue->heap = temp;
        queue->capacity = new_capacity;
    }

    // add the data to the end of the queue
    queue->heap[queue->size] = (void*)data;
    index = queue->size;
    parent = PARENT_AT(index);

    // while not at root and the data is greater than the parent
    while (index > 0 && queue->compare(queue->heap[index], queue->heap[parent]) > 0) {
        // swap the data with the parent
        temp = queue->heap[index];
        queue->heap[index] = queue->heap[parent];
        queue->heap[parent] = temp;

        // update the index and parent
        index = parent;
        parent = PARENT_AT(index);
    }

    // update the size
    queue->size++;
    return 0;
}

int priority_queue_extract(priority_queue* queue, void** data) {
    void* temp;
    int index, left_child, right_child, largest;

    // if queue does not exist or is empty
    if (queue == NULL || queue->size == 0) {
        return -1;
    }

    // set the data to the root
    *data = queue->heap[0];
    queue->heap[0] = queue->heap[queue->size - 1];
    queue->size--;

    // heapify
    index = 0;
    while (true) {
        // fetch the left and right children
        left_child = LEFT_CHILD_OF(index);
        right_child = RIGHT_CHILD_OF(index);

        // find the largest child
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

        // swap the data
        temp = queue->heap[index];
        queue->heap[index] = queue->heap[largest];
        queue->heap[largest] = temp;
        index = largest;
    }

    // if the size is less than a quarter of the capacity
    if (queue->size > 0 && queue->size == queue->capacity / 4) {
        // calculate new capacity
        int new_capacity = queue->capacity / 2;

        // reallocate memory
        temp = realloc(queue->heap, new_capacity * sizeof(void*));

        // if reallocation fails
        if (temp == NULL) {
            return -1;
        }

        // update the queue
        queue->heap = temp;
        queue->capacity = new_capacity;
    }

    return 0;
}

void* priority_queue_peek(const priority_queue* queue) {
    // if queue does not exist or is empty
    if (queue == NULL || queue->size == 0) {
        return NULL;
    }

    // return the root
    return queue->heap[0];
}

int priority_queue_size(const priority_queue* queue) {
    // if queue does not exist
    if (queue == NULL) {
        return -1;
    }

    // return the size
    return queue->size;
}

bool priority_queue_is_empty(const priority_queue* queue) {
    // if queue does not exist
    if (queue == NULL) {
        return false;
    }

    // return if the size is 0
    return queue->size == 0;
}