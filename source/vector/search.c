/// @file vector/search.c

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "common.h"
#include "access.h"

inline size_t vector_find_z(
    vector_c vector,
    bool (*eqf)(const void *a, const void *b),
    const void *data,
    size_t z) {
  return vector_find_next_z(vector, 0, eqf, data, z);
}

inline size_t vector_find_next_z(
    vector_c vector,
    size_t i,
    bool (*eqf)(const void *a, const void *b),
    const void *data,
    size_t z) {
  do {
    if (eqf(vector_at(vector, i, z), data))
      return i;
  } while (++i < vector_length(vector));
  return SIZE_MAX;
}

inline size_t vector_find_last_z(
    vector_c vector,
    size_t i,
    bool (*eqf)(const void *a, const void *b),
    const void *data,
    size_t z) {
  while (i-- > 0) {
    if (eqf(vector_at(vector, i, z), data))
      return i;
  }
  return SIZE_MAX;
}
