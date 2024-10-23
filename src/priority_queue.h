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

/**
 * Creates a new priority queue.
 * @param destroy The function to destroy the data in the queue.
 * @param compare The function to compare the data in the queue.
 * @return The new priority queue.
 */
priority_queue* priority_queue_create(void (*destroy)(void* data),
                                       int (*compare)(const void* key1, const void* key2));

/**
 * Destroys the priority queue.
 * @param queue The priority queue to destroy.
 */
void priority_queue_destroy(priority_queue* queue);

/**
 * Inserts data into the priority queue.
 * @param queue The priority queue.
 * @param data The data to insert.
 * @return 0 if successful, -1 if an error occurs.
 */
int priority_queue_insert(priority_queue* queue, const void* data);

/**
 * Extracts the data with the highest priority from the queue.
 * @param queue The priority queue.
 * @param data The extracted data.
 * @return 0 if successful, -1 if an error occurs.
 */
int priority_queue_extract(priority_queue* queue, void** data);

/**
 * Peeks at the data with the highest priority in the queue.
 * @param queue The priority queue.
 * @return The data with the highest priority.
 */
void* priority_queue_peek(const priority_queue* queue);

/**
 * Returns the size of the priority queue.
 * @param queue The priority queue.
 * @return The size of the priority queue.
 */
int priority_queue_size(const priority_queue* queue);

/**
 * Checks if the priority queue is empty.
 * @param queue The priority queue.
 * @return True if the priority queue is empty, false otherwise.
 */
bool priority_queue_is_empty(const priority_queue* queue);

#endif // PRIORITY_QUEUE_H
