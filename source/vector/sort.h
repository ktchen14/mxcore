/// @file vector/sort.h

#ifndef VECTOR_SORT_H
#define VECTOR_SORT_H

#include <stddef.h>
#include "common.h"

/// @addtogroup vector_module Vector
/// @{
/// @name Sorting
/// @{

//= void vector_sort(vector_t vector, int (*cmpf)(const void *a, const void *b))
#define vector_sort(v, ...) vector_sort_z((v), __VA_ARGS__, VECTOR_Z((v)))

/// Sort the @a vector according to @a cpmf
void vector_sort_z(
    vector_t vector,
    int (*cmpf)(const void *a, const void *b),
    size_t z)
  __attribute__((nonnull));

/// @}
/// @}

#endif /* VECTOR_SORT_H */

#ifndef VECTOR_TEST
#include "sort.c"
#endif /* VECTOR_TEST */
