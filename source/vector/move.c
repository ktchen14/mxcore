/// @file vector/move.c

#ifndef VECTOR_MOVE_C
#define VECTOR_MOVE_C

#include <stddef.h>
#include <stdlib.h>

#include "common.h"
#include "move.h"
#include "access.h"

VECTOR_INLINE
void vector_move_z(vector_t vector, size_t target, size_t source, size_t z) {
  if (target == source)
    return;

  if (target < source) {
    while (source-- > target)
      vector_swap_z(vector, source, source + 1, z);
  } else {
    for (; source < target; source++)
      vector_swap_z(vector, source, source + 1, z);
  }
}

VECTOR_INLINE
void vector_swap_z(vector_t vector, size_t i, size_t j, size_t z) {
  char *a = vector_at(vector, i, z);
  char *b = vector_at(vector, j, z);

  for (size_t k = 0; k < z; k++) {
    char buffer;
    buffer = a[k];
    a[k] = b[k];
    b[k] = buffer;
  }
}

VECTOR_INLINE
void vector_sort_z(
    vector_t vector,
    int (*cmpf)(const void *a, const void *b),
    size_t z) {
  qsort(vector, vector_length(vector), z, cmpf);
}

#endif /* VECTOR_MOVE_C */
