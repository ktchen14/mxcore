/// @file header/vector/comparison.c

#ifndef VECTOR_COMPARISON_C
#define VECTOR_COMPARISON_C

#include <stdbool.h>
#include <stddef.h>

#include "common.h"
#include "comparison.h"
#include "access.h"

#ifdef VECTOR_TEST
#define inline
#endif /* VECTOR_TEST */

inline static __attribute__((nonnull(1, 2), unused))
bool __vector_data_as_eq(const void *a, const void *b, void *data) {
  return ((bool (*)(const void *a, const void *b)) data)(a, b);
}

inline bool vector_eq_z(
    vector_c va,
    vector_c vb,
    bool (*eq)(const void *a, const void *b),
    size_t za,
    size_t zb) {
  return vector_eq_with_z(va, vb, __vector_data_as_eq, eq, za, zb);
}

inline bool vector_eq_with_z(
    vector_c va,
    vector_c vb,
    bool (*eq)(const void *a, const void *b, void *data),
    void *data,
    size_t za,
    size_t zb) {
  if (vector_length(va) != vector_length(vb))
    return false;

  for (size_t i = 0; i < vector_length(va); i++) {
    if (!eq(vector_at(va, i, za), vector_at(vb, i, zb), data))
      return false;
  }

  return true;
}

#ifdef VECTOR_TEST
#undef inline
#endif /* VECTOR_TEST */

#endif /* VECTOR_COMPARISON_C */
