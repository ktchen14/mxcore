/// @file header/vector/delete.c

#ifndef VECTOR_DELETE_C
#define VECTOR_DELETE_C

#include <stdlib.h>

#include "common.h"
#include "delete.h"

#ifdef VECTOR_TEST
#define inline
#endif /* VECTOR_TEST */

inline void *vector_delete(vector_t vector) {
  return free(__vector_to_header(vector)), NULL;
}

#ifdef VECTOR_TEST
#undef inline
#endif /* VECTOR_TEST */

#endif /* VECTOR_DELETE_C */
