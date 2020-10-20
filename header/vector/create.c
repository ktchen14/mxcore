/// @file header/vector/create.c

#ifndef VECTOR_CREATE_C
#define VECTOR_CREATE_C

#include <errno.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "create.h"

#ifdef VECTOR_TEST
#define inline
#endif /* VECTOR_TEST */

inline vector_t vector_create(void) {
  struct __vector_header_t *header;

  if ((header = malloc(sizeof(*header))) == NULL)
    return NULL;

  header->volume = 0;
  header->length = 0;
  return header->data;
}

inline vector_t vector_import_z(const void *data, size_t length, size_t z) {
  struct __vector_header_t *header;

  // Doesn't overflow because this is the size of data
  size_t size = length * z;
  if (__builtin_add_overflow(size, sizeof(*header), &size))
    return errno = ENOMEM, NULL;
  if ((header = malloc(size)) == NULL)
    return NULL;

  header->volume = length;
  header->length = length;
  return memcpy(header->data, data, length * z);
}

#ifdef VECTOR_TEST
#undef inline
#endif /* VECTOR_TEST */

#endif /* VECTOR_CREATE_C */
