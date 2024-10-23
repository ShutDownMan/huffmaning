#ifndef DYNAMIC_ARRAY
#define DYNAMIC_ARRAY

#include <stdlib.h>

/**
 * Structure to represent a dynamic array
 */
typedef struct dynamic_array {
  void **array;   // The array of elements
  int size;       // The number of elements in the array
  int capacity;   // The capacity of the array
} dynamic_array;

/**
 * Function to create a dynamic array
 * @return The dynamic array
 */
dynamic_array *dynamic_array_create();

/**
 * Function to destroy a dynamic array
 * @param array The dynamic array
 * @param destroy The function to destroy the elements
 */
void dynamic_array_destroy(dynamic_array *array, void (*destroy)(void *element));

/**
 * Function to insert an element into a dynamic array
 * @param array The dynamic array
 * @param element The element to insert
 */
void dynamic_array_insert(dynamic_array *array, void *element);

/**
 * Function to get an element from a dynamic array
 * @param array The dynamic array
 * @param index The index of the element
 * @return The element
 */
void *dynamic_array_get(dynamic_array *array, int index);

/**
 * Function to remove an element from a dynamic array
 * @param array The dynamic array
 * @param index The index of the element
 */
void dynamic_array_remove(dynamic_array *array, int index);

/**
 * Function to get the size of a dynamic array
 * @param array The dynamic array
 * @return The size of the array
 */
int dynamic_array_size(dynamic_array *array);

/**
 * Function to get the capacity of a dynamic array
 * @param array The dynamic array
 * @return The capacity of the array
 */
int dynamic_array_capacity(dynamic_array *array);

#endif // DYNAMIC_ARRAY