/// @file header/vector/access.h

#ifndef VECTOR_ACCESS_H
#define VECTOR_ACCESS_H

#include <stddef.h>
#include "common.h"

#ifdef VECTOR_TEST
#define inline
#endif /* VECTOR_TEST */

/**
 * @brief Return a pointer to the element in the @a vector at index @a i
 *
 * @note Though this operation doesn't have the @c _z suffix, it @b is a part of
 * the explicit interface and takes the element size of the @a vector as @a z.
 * This operation is redundant if the element type of the @a vector is known at
 * compile time as it's identical to <code>vector + i</code>.
 *
 * @note This operation is implemented as a macro but documented as a function
 * to clarify its intended usage.
 *
 * If @a i is the length of the @a vector then this will return a pointer to
 * just past the last element of the @a vector.
 *
 * This operation is @c const qualified on the @a vector. That is if the element
 * type of the @a vector is @c const qualified (@a vector is compatible with a
 * @ref vector_c), this will return a <tt>const void *</tt>. Otherwise this will
 * return a <tt>void *</tt>.
 *
 * This is the inverse of vector_index() such that:
 * @code{.c}
 *   vector_index(vector, vector_at(i)) == i
 *   vector_at(vector, vector_index(elmt)) == elmt
 * @endcode
 *
 * If @a i is neither an index in the @a vector or its length then the behavior
 * is undefined.
 *
 * @see vector_index() - the inverse operation to get the index of an element in
 *   a vector
 */
//= void *vector_at(vector_t vector, size_t i, size_t z)
#define vector_at(vector, i, z) ({ \
  /* Warn or fail if vector isn't a pointer. When this is absent if vector */ \
  /* is a scalar no warning is issued due to the explicit cast to char *. */ \
  const void *__vector = (vector); \
  _Pragma("GCC diagnostic push") \
  _Pragma("GCC diagnostic ignored \"-Wcast-align\"") \
  _Pragma("GCC diagnostic ignored \"-Wcast-qual\"") \
  (__typeof__((vector))) ((const char *) (__vector) + (i) * (z)); \
  _Pragma("GCC diagnostic pop") \
})

/**
 * @brief Return a pointer to the last element in the @a vector
 *
 * @note This operation is implemented as a macro but documented as a function
 * to clarify its intended usage.
 *
 * This operation is @c const qualified on the @a vector. That is if the element
 * type of the @a vector is @c const qualified (@a vector is compatible with a
 * @ref vector_c) then this will return a <tt>const void *</tt>. Otherwise this
 * will return a <tt>void *</tt>.
 *
 * If no last element is in the @a vector (the @a vector's @length is zero) then
 * the behavior is undefined.
 *
 * @see vector_tail_z() - the explicit interface analogue
 *
 * @remark No <code>vector_head()</code> or <code>vector_head_z()</code> is
 * available; a pointer to the first element in some vector is just @c vector.
 */
//= void *vector_tail(vector_t vector)
#define vector_tail(v) vector_tail_z((v), VECTOR_Z(v))

/**
 * @brief Return a pointer to the last element in the @a vector
 *
 * @note This operation is implemented as a macro but documented as a function
 * to clarify its intended usage.
 *
 * This operation is @c const qualified on the @a vector. That is if the element
 * type of the @a vector is @c const qualified (@a vector is compatible with a
 * @ref vector_c) then this will return a <tt>const void *</tt>. Otherwise this
 * will return a <tt>void *</tt>.
 *
 * If no last element is in the @a vector (the @a vector's @length is zero) then
 * the behavior is undefined.
 *
 * @see vector_tail() - the implicit interface analogue
 *
 * @remark No <code>vector_head()</code> or <code>vector_head_z()</code> is
 * available; a pointer to the first element in some vector is just @c vector.
 */
//= void *vector_tail_z(vector_t vector, size_t z)
#define vector_tail_z(v, z) vector_at((v), vector_length((v)) - 1, (z))

/**
 * @brief Return the index of the element at @a elmt in the @a vector
 *
 * @note Though this operation doesn't have the @c _z suffix, it @b is a part of
 * the explicit interface and takes the element size of the @a vector as @a z.
 * This operation is redundant if the element type of the @a vector is known at
 * compile time as it's identical to <code>elmt - vector</code>.
 *
 * This doesn't inspect the data at @a elmt or the elements in the @a vector;
 * @a elmt must already be a pointer to an element in the @a vector.
 *
 * This is the inverse of vector_at() such that:
 * @code{.c}
 *   vector_at(vector, vector_index(elmt)) == elmt
 *   vector_index(vector, vector_at(i)) == i
 * @endcode
 *
 * If @a elmt is @c NULL or isn't a pointer to an element in the @a vector then
 * the behavior is undefined. If @a elmt is a pointer to an offset in an element
 * in the @a vector then the behavior is undefined.
 *
 * @see vector_at() - the inverse operation to get a pointer to an element in a
 *   vector
 */
inline size_t vector_index(vector_c vector, const void *elmt, size_t z)
  __attribute__((nonnull, pure));

/**
 * @brief Copy the element at index @a i in the @a vector into @a elmt
 *
 * @note Though this operation doesn't have the @c _z suffix, it @b is a part of
 * the explicit interface and takes the element size of the @a vector as @a z.
 * This operation is redundant if the element type of the @a vector is known at
 * compile time as it's identical to <code>*elmt = vector[i]</code>.
 *
 * If @a i isn't an index in the @a vector or @a elmt is @c NULL then the
 * behavior is undefined.
 *
 * @param vector the vector to operate on
 * @param i the index of the element in the @a vector to copy from
 * @param elmt the location to copy the element to
 * @param z the element size of the @a vector
 *
 * @see vector_set() - the inverse operation to copy data into a vector
 */
inline void vector_get(vector_c vector, size_t i, void *elmt, size_t z)
  __attribute__((nonnull));

/**
 * @brief Copy the object at @a elmt into the @a vector at index @a i
 *
 * @note Though this operation doesn't have the @c _z suffix, it @b is a part of
 * the explicit interface and takes the element size of the @a vector as @a z.
 * This operation is redundant if the element type of the @a vector is known at
 * compile time as it's identical to <code>vector[i] = *elmt</code>.
 *
 * If @a i isn't an index in the @a vector, @a elmt is @c NULL, or the type of
 * the object at @a elmt is incompatible with the element type of the vector,
 * then the behavior is undefined.
 *
 * @param vector the vector to operate on
 * @param i the index of the element in the @a vector to copy to
 * @param elmt the location to copy the element from
 * @param z the element size of the @a vector
 *
 * @see vector_get() - the inverse operation to copy data from a vector
 */
inline void vector_set(vector_t vector, size_t i, const void *elmt, size_t z)
  __attribute__((nonnull));

#ifdef VECTOR_TEST
#undef inline
#endif /* VECTOR_TEST */

#endif /* VECTOR_ACCESS_H */

#ifndef VECTOR_TEST
#include "access.c"
#endif /* VECTOR_TEST */
