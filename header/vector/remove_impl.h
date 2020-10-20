/// @file header/vector/remove.c

#ifndef VECTOR_REMOVE_C
#define VECTOR_REMOVE_C

#include <stddef.h>
#include <string.h>

#include "common.h"
#include "remove.h"
#include "access.h"
#include "resize.h"

#ifdef VECTOR_TEST
#define inline
#endif /* VECTOR_TEST */

inline vector_t vector_remove_z(vector_t vector, size_t i, size_t z) {
  return vector_excise_z(vector, i, 1, z);
}

inline vector_t vector_excise_z(vector_t vector, size_t i, size_t n, size_t z) {
  size_t length = vector_length(vector) - n;

  // move the existing elements n elements toward the head
  void *target = vector_at(vector, i + 0, z);
  void *source = vector_at(vector, i + n, z);
  size_t size = (length - i) * z;
  memmove(target, source, size);

  if (length <= (vector_volume(vector) - 1) / 2) {
    vector_t resize;
    // just volume = (length * 6 + 4) / 5 avoiding intermediate overflow
    size_t volume = length / 5 * 6 + ((length % 5) * 6 + 4) / 5;

    if ((resize = vector_resize_z(vector, volume, z)) != NULL)
      vector = resize;
  }

  // decrease the length
  __vector_to_header(vector)->length = length;

  return vector;
}

inline vector_t vector_truncate_z(vector_t vector, size_t length, size_t z) {
  size_t n = vector_length(vector) - length;
  return vector_excise_z(vector, vector_length(vector) - n, n, z);
}

#ifdef VECTOR_TEST
#undef inline
#endif /* VECTOR_TEST */

#endif /* VECTOR_REMOVE_C */
