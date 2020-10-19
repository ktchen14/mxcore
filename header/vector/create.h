/// @file header/vector/create.h

#ifndef VECTOR_CREATE_H
#define VECTOR_CREATE_H

#include <stddef.h>
#include "common.h"

#ifdef VECTOR_TEST
#define inline
#endif /* VECTOR_TEST */

/**
 * @brief Allocate and initialize a zero length vector
 *
 * On failure this will retain the value of @c errno set by malloc().
 *
 * @return the new vector on success; otherwise @c NULL
 */
inline vector_t vector_create(void) __attribute__((__malloc__));

/**
 * @brief Allocate and initialize a vector from @a length elements of @a data
 *
 * This will create and return a vector containing @a length elements from
 * @a data. On failure this will retain the value of @c errno set by malloc().
 *
 * @param data the data to initialize the vector from
 * @param length the number of elements to copy from @a data
 * @return the new vector on success; otherwise @c NULL
 *
 * @see vector_import_z() - the explicit interface analogue
 */
//= vector_t vector_import(const void *data, size_t length)
#define vector_import(data, length) \
  vector_import_z((data), (length), VECTOR_Z((data)))

/**
 * @brief Allocate and initialize a vector from @a length elements of @a data
 *
 * This will create and return a vector containing @a length elements from
 * @a data. On failure this will retain the value of @c errno set by malloc().
 *
 * @param data the data to initialize the vector from
 * @param length the number of elements to copy from @a data
 * @param z the element size of @a data
 * @return the new vector on success; otherwise @c NULL
 *
 * @see vector_import() - the implicit interface analogue
 */
inline vector_t vector_import_z(const void *data, size_t length, size_t z)
  __attribute__((__malloc__, nonnull));

/**
 * @brief Allocate and initialize a vector from the argument list
 *
 * This is vector_import() with @a data constructed and @a length calculated
 * from the argument list. The element type of the created vector will be
 * @a type. If an argument in ... is incompatible with @a type then the behavior
 * is undefined.
 *
 * For example: @code{.c}
 *   int *vector = vector_define(int, 1, 2, 3, 5, 8, 13, 21, 34);
 * @endcode
 *
 * On failure this will retain the value of @c errno set by malloc().
 *
 * @param type a complete object type
 * @param ... a sequence of elements to initialize the vector from
 * @return the new vector on success; otherwise @c NULL
 */
#define vector_define(type, ...) ({ \
  /* Fail unless type is an actual type before we declare __data with */ \
  /* __typeof__(type). Otherwise the compiler is silent on this kind of */ \
  /* mistake: */ \
  /*   vector_define(1, 2, 3, 4)       => (int[])    { 2, 3, 4 } */ \
  /* When this was intended: */ \
  /*   vector_define(int, 1, 2, 3, 4)  => (int[]) { 1, 2, 3, 4 } */ \
  (void) __builtin_types_compatible_p(type, void); \
  /* We must take the __typeof__(type) here so that a strange type like */ \
  /* int[2] or void (*)(void) is syntactically acceptable. */ \
  __typeof__(type) __data[] = { __VA_ARGS__ }; \
  vector_import(__data, sizeof(__data) / sizeof(__data[0])); \
})

#ifdef VECTOR_TEST
#undef inline
#endif /* VECTOR_TEST */

#endif /* VECTOR_CREATE_H */

#ifndef VECTOR_TEST
#include "create.c"
#endif /* VECTOR_TEST */
