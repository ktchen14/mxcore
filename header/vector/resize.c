/// @file header/vector/resize.c

#ifndef VECTOR_RESIZE_C
#define VECTOR_RESIZE_C

#include <errno.h>
#include <stdlib.h>

#include "common.h"
#include "resize.h"

#ifdef VECTOR_TEST
#define inline
#endif /* VECTOR_TEST */

inline vector_t vector_resize_z(vector_t vector, size_t volume, size_t z) {
  struct __vector_header_t *header = __vector_to_header(vector);
  size_t size;

  // calculate size and test for overflow
  if (__builtin_mul_overflow(volume, z, &size))
    return errno = ENOMEM, NULL;
  if (__builtin_add_overflow(size, sizeof(*header), &size))
    return errno = ENOMEM, NULL;

  if ((header = realloc(header, size)) == NULL)
    return NULL;

  if ((header->volume = volume) < header->length)
    header->length = volume;

  return header->data;
}

inline vector_t vector_shrink_z(vector_t vector, size_t z) {
  vector_t shrunk;

  shrunk = vector_resize_z(vector, vector_length(vector), z);

  return shrunk != NULL ? shrunk : vector;
}

inline vector_t vector_ensure_z(vector_t vector, size_t length, size_t z) {
  if (length <= vector_volume(vector))
    return vector;

  // just volume = (length * 8 + 3) / 5 avoiding intermediate overflow
  size_t volume = length / 5 * 8 + ((length % 5) * 8 + 3) / 5;

  // if the volume doesn't overflow then attempt to allocate it
  if (volume > length) {
    vector_t resize;
    if ((resize = vector_resize_z(vector, volume, z)) != NULL)
      return resize;
  }

  // if either the volume overflows or the allocation failed then attempt to
  // resize to just the length
  return vector_resize_z(vector, length, z);
}

#ifdef VECTOR_TEST
#undef inline
#endif /* VECTOR_TEST */

#endif /* VECTOR_RESIZE_C */
