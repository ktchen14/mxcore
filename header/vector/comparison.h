/// @file header/vector/comparison.h

#ifndef VECTOR_COMPARISON_H
#define VECTOR_COMPARISON_H

#include <stddef.h>
#include "common.h"

#ifdef VECTOR_TEST
#define inline
#endif /* VECTOR_TEST */

/// @addtogroup vector_module Vector
/// @{
/// @name Comparison
/// @{

/**
 * @brief Return whether vector @a va is equivalent to vector @a vb
 *
 * Here "equivalent" means that both of @a va and @a vb are @c NULL, or neither
 * @a va or @a vb is @c NULL and:
 *
 * - @a va and @a vb have the same length, and
 * - @a eq returns @c true when called on each element in @a va and the element
 *   at the same index in @a vb
 *
 * @param va a vector to operate on
 * @param vb a vector to operate on
 * @param eq the equality function that will be used to decide whether an
 *   element in @a va is equivalent to an element in @a vb
 * @return whether vector @a va is equivalent to vector @a vb
 */
//= _Bool vector_eq(
//=     vector_c va, vector_c vb, _Bool (*eq)(const void *a, const void *b))
#define vector_eq(va, vb, ...) \
  vector_eq_z((va), (vb), __VA_ARGS__, VECTOR_Z((va)), VECTOR_Z((vb)))

/**
 * @brief Return whether vector @a va is equivalent to vector @a vb
 *
 * Here "equivalent" means that both of @a va and @a vb are @c NULL, or neither
 * @a va or @a vb is @c NULL and:
 *
 * - @a va and @a vb have the same length, and
 * - @a eq returns @c true when called on each element in @a va and the element
 *   at the same index in @a vb
 *
 * @param va a vector to operate on
 * @param vb a vector to operate on
 * @param eq the equality function that will be used to decide whether an
 *   element in @a va is equivalent to an element in @a vb
 * @param za the element size of @a va
 * @param zb the element size of @a vb
 * @return whether vector @a va is equivalent to vector @a vb
 */
inline _Bool vector_eq_z(
    vector_c va,
    vector_c vb,
    _Bool (*eq)(const void *a, const void *b) __attribute__((nonnull)),
    size_t za,
    size_t zb)
  __attribute__((nonnull(3)));

/**
 * @brief Return whether vector @a va is equivalent to vector @a vb with
 *   contextual information
 *
 * Here "equivalent" means that both of @a va and @a vb are @c NULL, or neither
 * @a va or @a vb is @c NULL and:
 *
 * - @a va and @a vb have the same length, and
 * - @a eq returns @c true when called on each element in @a va and the element
 *   at the same index in @a vb
 *
 * @param va a vector to operate on
 * @param vb a vector to operate on
 * @param eq the equality function that will be used to decide whether an
 *   element in @a va is equivalent to an element in @a vb
 * @param data contextual information to pass as the last argument to @a eq
 * @return whether vector @a va is equivalent to vector @a vb
 */
//= _Bool vector_eq_with(
//=     vector_c va,
//=     vector_c vb,
//=     _Bool (*eq)(const void *a, const void *b, void *data),
//=     void *data)
#define vector_eq_with(va, vb, ...) \
  vector_eq_with_z((va), (vb), __VA_ARGS__, VECTOR_Z((va)), VECTOR_Z((vb)))

/**
 * @brief Return whether vector @a va is equivalent to vector @a vb with
 *   contextual information
 *
 * Here "equivalent" means that both of @a va and @a vb are @c NULL, or neither
 * @a va or @a vb is @c NULL and:
 *
 * - @a va and @a vb have the same length, and
 * - @a eq returns @c true when called on each element in @a va and the element
 *   at the same index in @a vb
 *
 * @param va a vector to operate on
 * @param vb a vector to operate on
 * @param eq the equality function that will be used to decide whether an
 *   element in @a va is equivalent to an element in @a vb
 * @param data contextual information to pass as the last argument to @a eq
 * @param za the element size of @a va
 * @param zb the element size of @a vb
 * @return whether vector @a va is equivalent to vector @a vb
 */
inline _Bool vector_eq_with_z(
    vector_c va,
    vector_c vb,
    _Bool (*eq)(const void *a, const void *b, void *data)
      __attribute__((nonnull(1, 2))),
    void *data,
    size_t za,
    size_t zb)
  __attribute__((nonnull(3)));

//= int vector_cmp(
//=     vector_c va, vector_c vb, int (*cmp)(const void *a, const void *b))
#define vector_cmp(va, vb, ...) \
  vector_cmp_z((va), (vb), __VA_ARGS__, VECTOR_Z((va)), VECTOR_Z((vb)))

inline int vector_cmp_z(
    vector_c va,
    vector_c vb,
    int (*cmp)(const void *a, const void *b),
    size_t za,
    size_t zb);

//= int vector_cmp_with(
//=     vector_c va,
//=     vector_c vb,
//=     int (*cmp)(const void *a, const void *b, void *data),
//=     void *data)
#define vector_cmp_with(va, vb, ...) \
  vector_cmp_with_z((va), (vb), __VA_ARGS__, VECTOR_Z((va)), VECTOR_Z((vb)))

inline int vector_cmp_with_z(
    vector_c va,
    vector_c vb,
    int (*cmp)(const void *a, const void *b, void *data),
    void *data,
    size_t za,
    size_t zb);

/// @}
/// @}

#ifdef VECTOR_TEST
#undef inline
#endif /* VECTOR_TEST */

#endif /* VECTOR_COMPARISON_H */

#ifndef VECTOR_TEST
#include "comparison.c"
#endif /* VECTOR_TEST */
