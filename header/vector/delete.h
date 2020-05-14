/// @file header/vector/delete.h

#ifndef VECTOR_DELETE_H
#define VECTOR_DELETE_H

#include "common.h"

#ifdef VECTOR_TEST
#define inline
#endif /* VECTOR_TEST */

/// Deallocate the @a vector and return @c NULL
inline void *vector_delete(vector_t vector);

#ifdef VECTOR_TEST
#undef inline
#endif /* VECTOR_TEST */

#endif /* VECTOR_DELETE_H */

#ifndef VECTOR_TEST
#include "delete.c"
#endif /* VECTOR_TEST */
