/// @file header/vector/shift.c

#ifndef VECTOR_SHIFT_C
#define VECTOR_SHIFT_C

#include <stddef.h>

#include "common.h"
#include "access.h"
#include "remove.h"

#ifdef VECTOR_TEST
#define inline
#endif /* VECTOR_TEST */

inline vector_t vector_pull_z(vector_t vector, void *elmt, size_t z) {
  if (elmt != NULL)
    vector_get(vector, vector_length(vector) - 1, elmt, z);
  return vector_remove_z(vector, vector_length(vector) - 1, z);
}

inline vector_t vector_shift_z(vector_t vector, void *elmt, size_t z) {
  if (elmt != NULL)
    vector_get(vector, 0, elmt, z);
  return vector_remove_z(vector, 0, z);
}

#ifdef VECTOR_TEST
#undef inline
#endif /* VECTOR_TEST */

#endif /* VECTOR_SHIFT_C */
