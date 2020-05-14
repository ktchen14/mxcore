/// @file header/vector/sort.c

#ifndef VECTOR_SORT_C
#define VECTOR_SORT_C

#include <stddef.h>
#include <stdlib.h>

#include "common.h"
#include "sort.h"

#ifdef VECTOR_TEST
#define inline
#endif /* VECTOR_TEST */

inline void vector_sort_z(
    vector_t vector,
    int (*cmp)(const void *a, const void *b),
    size_t z) {
  qsort(vector, vector_length(vector), z, cmp);
}

#ifdef VECTOR_TEST
#undef inline
#endif /* VECTOR_TEST */

#endif /* VECTOR_SORT_C */
