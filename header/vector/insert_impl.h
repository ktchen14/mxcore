/// @file header/vector/insert.c

#ifndef VECTOR_INSERT_C
#define VECTOR_INSERT_C

#include <errno.h>
#include <stddef.h>
#include <string.h>

#include "common.h"
#include "insert.h"
#include "access.h"
#include "resize.h"

#ifdef VECTOR_TEST
#define inline
#endif /* VECTOR_TEST */

inline vector_t vector_insert_z(
    restrict vector_t vector,
    size_t i,
    const void *restrict elmt,
    size_t z) {
  return vector_inject_z(vector, i, elmt, 1, z);
}

inline vector_t vector_inject_z(
    restrict vector_t vector,
    size_t i,
    const void *restrict elmt,
    size_t n,
    size_t z) {
  size_t length = vector_length(vector);

  if (__builtin_add_overflow(length, n, &length))
    return errno = ENOMEM, NULL;

  if ((vector = vector_ensure_z(vector, length, z)) == NULL)
    return NULL;

  // move the existing elements n elements toward the tail
  void *target = vector_at(vector, i + n, z);
  void *source = vector_at(vector, i + 0, z);
  size_t size = (vector_length(vector) - i) * z;
  memmove(target, source, size);

  if (elmt != NULL)
    memcpy(vector_at(vector, i, z), elmt, n * z);

  // increase the length
  __vector_to_header(vector)->length = length;

  return vector;
}

inline vector_t vector_append_z(
    restrict vector_t vector,
    const void *restrict elmt,
    size_t z) {
  return vector_inject_z(vector, vector_length(vector), elmt, 1, z);
}

inline vector_t vector_extend_z(
    restrict vector_t vector,
    const void *restrict elmt,
    size_t n,
    size_t z) {
  return vector_inject_z(vector, vector_length(vector), elmt, n, z);
}

#ifdef VECTOR_TEST
#undef inline
#endif /* VECTOR_TEST */

#endif /* VECTOR_INSERT_C */
