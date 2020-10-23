/// @file header/vector/search.c

#ifndef VECTOR_SEARCH_C
#define VECTOR_SEARCH_C

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "common.h"
#include "search.h"
#include "access.h"

#ifdef VECTOR_TEST
#define inline
#endif /* VECTOR_TEST */

inline size_t vector_find_z(
    vector_c vector,
    _Bool (*eqf)(const void *elmt, const void *data),
    const void *data,
    size_t z) {
  return vector_find_next_z(vector, 0, eqf, data, z);
}

inline size_t vector_find_next_z(
    vector_c vector,
    size_t i,
    _Bool (*eqf)(const void *elmt, const void *data),
    const void *data,
    size_t z) {
  for (; i < vector_length(vector); i++) {
    if (eqf(vector_at(vector, i, z), data))
      return i;
  }
  return SIZE_MAX;
}

inline size_t vector_find_last_z(
    vector_c vector,
    size_t i,
    _Bool (*eqf)(const void *elmt, const void *data),
    const void *data,
    size_t z) {
  while (i-- > 0) {
    if (eqf(vector_at(vector, i, z), data))
      return i;
  }
  return SIZE_MAX;
}

inline size_t vector_search_z(
    vector_c vector,
    const void *elmt,
    int (*cmpf)(const void *a, const void *b),
    size_t z) {
  size_t length = vector_length(vector);
  void *result;

  if ((result = bsearch(elmt, vector, length, z, cmpf)) == NULL)
    return SIZE_MAX;

  size_t i = vector_index(vector, result, z);

  while (i > 0 && cmpf(vector_at(vector, i - 1, z), elmt) == 0)
    i--;

  return i;
}

#ifdef VECTOR_TEST
#undef inline
#endif /* VECTOR_TEST */

#endif /* VECTOR_SEARCH_C */
