/// @file header/vector/move.c

#ifndef VECTOR_MOVE_C
#define VECTOR_MOVE_C

#include <stddef.h>
#include <stdlib.h>

#include "common.h"
#include "move.h"
#include "access.h"

#ifdef VECTOR_TEST
#define inline
#endif /* VECTOR_TEST */

inline void vector_move_z(
    vector_t vector, size_t target, size_t source, size_t z) {
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

inline void vector_swap_z(vector_t vector, size_t i, size_t j, size_t z) {
  char *a = vector_at(vector, i, z);
  char *b = vector_at(vector, j, z);

  for (size_t k = 0; k < z; k++) {
    char buffer;
    buffer = a[k];
    a[k] = b[k];
    b[k] = buffer;
  }
}

#ifdef VECTOR_TEST
#undef inline
#endif /* VECTOR_TEST */

#endif /* VECTOR_MOVE_C */
