/// @file header/vector/comparison.c

#ifndef VECTOR_COMPARISON_C
#define VECTOR_COMPARISON_C

#include <stddef.h>

#include "common.h"
#include "comparison.h"
#include "access.h"

#ifdef VECTOR_TEST
#define inline
#endif /* VECTOR_TEST */

static inline __attribute__((nonnull(1, 2), unused))
_Bool __vector_data_as_eq(const void *a, const void *b, void *data) {
  _Bool (*eq)(const void *a, const void *b) __attribute__((nonnull)) = data;
  return eq(a, b);
}

inline _Bool vector_eq_z(
    vector_c va,
    vector_c vb,
    _Bool (*eq)(const void *a, const void *b),
    size_t za,
    size_t zb) {
  return vector_eq_with_z(va, vb, __vector_data_as_eq, eq, za, zb);
}

inline _Bool vector_eq_with_z(
    vector_c va,
    vector_c vb,
    _Bool (*eq)(const void *a, const void *b, void *data),
    void *data,
    size_t za,
    size_t zb) {
  if (va == NULL && vb == NULL)
    return 1;
  if (va == NULL || vb == NULL)
    return 0;

  if (vector_length(va) != vector_length(vb))
    return 0;

  for (size_t i = 0; i < vector_length(va); i++) {
    if (!eq(vector_at(va, i, za), vector_at(vb, i, zb), data))
      return 0;
  }

  return 1;
}

#ifdef VECTOR_TEST
#undef inline
#endif /* VECTOR_TEST */

#endif /* VECTOR_COMPARISON_C */
