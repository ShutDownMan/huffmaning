#include "dynamic_array.h"

#include <stdlib.h>

dynamic_array *dynamic_array_create() {
  dynamic_array *array = malloc(sizeof(dynamic_array));
  array->array = malloc(2 * sizeof(void *));
  array->size = 0;
  array->capacity = 2;
  return array;
}

void dynamic_array_destroy(dynamic_array *array, void (*destroy)(void *element)) {
  if (array == NULL) {
    return;
  }
  if (destroy != NULL) {
    for (int i = 0; i < array->size; i++) {
      destroy(array->array[i]);
    }
  }
  free(array->array);
  free(array);
}

void dynamic_array_insert(dynamic_array *array, void *element) {
  if (array->size == array->capacity) {
    array->capacity *= 2;
    array->array = realloc(array->array, array->capacity * sizeof(void *));
  }
  array->array[array->size++] = element;
}

void *dynamic_array_get(dynamic_array *array, int index) {
  if (index < 0 || index >= array->size) {
    return NULL;
  }
  return array->array[index];
}

void dynamic_array_remove(dynamic_array *array, int index) {
  if (index < 0 || index >= array->size) {
    return;
  }
  for (int i = index; i < array->size - 1; i++) {
    array->array[i] = array->array[i + 1];
  }
  array->size--;
}

int dynamic_array_size(dynamic_array *array) {
  return array->size;
}

int dynamic_array_capacity(dynamic_array *array) {
  return array->capacity;
}
