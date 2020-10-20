/// @file header/vector/shift.h

#ifndef VECTOR_SHIFT_H
#define VECTOR_SHIFT_H

#include <stddef.h>
#include "common.h"

#ifdef VECTOR_TEST
#define inline
#endif /* VECTOR_TEST */

/// @copydoc vector_append()
#define vector_push vector_append

/// @copydoc vector_append_z()
#define vector_push_z vector_append_z

/**
 * @brief Copy the last element in the @a vector to @a elmt and remove it
 *
 * If @a elmt is @c NULL then the element won't be copied before it's removed.
 *
 * Once the element is removed, if the @length of the @a vector is reduced such
 * that <code>length <= (volume - 1) / 2</code>, a vector_resize() will be
 * attempted to reduce the @volume to:
 *   @f[ volume = \frac{length \times 6 + 4}{5} @f]
 * On success the shrunk vector will be returned. Otherwise the vector will be
 * returned as is (without the element).
 *
 * If no last element is in the @a vector (the @a vector's @length is zero) then
 * the behavior is undefined.
 *
 * @param vector the vector to operate on
 * @param elmt the location to copy the element to or @c NULL
 * @return the resultant vector
 */
//= vector_t vector_pull(vector_t vector, void *elmt)
#define vector_pull(v, ...) vector_pull_z((v), __VA_ARGS__, VECTOR_Z((v)))

/**
 * @brief Copy the last element in the @a vector to @a elmt and remove it
 *
 * If @a elmt is @c NULL then the element won't be copied before it's removed.
 *
 * Once the element is removed, if the @length of the @a vector is reduced such
 * that <code>length <= (volume - 1) / 2</code>, a vector_resize_z() will be
 * attempted to reduce the @volume to:
 *   @f[ volume = \frac{length \times 6 + 4}{5} @f]
 * On success the shrunk vector will be returned. Otherwise the vector will be
 * returned as is (without the element).
 *
 * If no last element is in the @a vector (the @a vector's @length is zero) then
 * the behavior is undefined.
 *
 * @param vector the vector to operate on
 * @param elmt the location to copy the element to or @c NULL
 * @param z the element size of the @a vector
 * @return the resultant vector
 */
inline vector_t vector_pull_z(vector_t vector, void *elmt, size_t z)
  __attribute__((nonnull(1), warn_unused_result));

/**
 * @brief Copy the first element in the @a vector to @a elmt and remove it
 *
 * All elements after the first are shifted one element toward the head of the
 * @a vector.
 *
 * If @a elmt is @c NULL then the element won't be copied before it's removed.
 *
 * Once the element is removed and subsequent elements are shifted, if the
 * @length of the @a vector is reduced such that <code>length <= (volume - 1) /
 * 2</code>, a vector_resize_z() will be attempted to reduce the @volume to:
 *   @f[ volume = \frac{length \times 6 + 4}{5} @f]
 * On success the shrunk vector will be returned. Otherwise the vector will be
 * returned as is (without the element).
 *
 * If no first element is in the @a vector (the @a vector's @length is zero)
 * then the behavior is undefined.
 *
 * @param vector the vector to operate on
 * @param elmt the location to copy the element to or @c NULL
 * @return the resultant vector
 */
//= vector_t vector_shift(vector_t vector, void *elmt)
#define vector_shift(v, ...) vector_shift_z((v), __VA_ARGS__, VECTOR_Z((v)))

/**
 * @brief Copy the first element in the @a vector to @a elmt and remove it
 *
 * All elements after the first are shifted one element toward the head of the
 * @a vector.
 *
 * If @a elmt is @c NULL then the element won't be copied before it's removed.
 *
 * Once the element is removed and subsequent elements are shifted, if the
 * @length of the @a vector is reduced such that <code>length <= (volume - 1) /
 * 2</code>, a vector_resize_z() will be attempted to reduce the @volume to:
 *   @f[ volume = \frac{length \times 6 + 4}{5} @f]
 * On success the shrunk vector will be returned. Otherwise the vector will be
 * returned as is (without the element).
 *
 * If no first element is in the @a vector (the @a vector's @length is zero)
 * then the behavior is undefined.
 *
 * @param vector the vector to operate on
 * @param elmt the location to copy the element to or @c NULL
 * @param z the element size of the @a vector
 * @return the resultant vector
 */
inline vector_t vector_shift_z(vector_t vector, void *elmt, size_t z)
  __attribute__((nonnull(1), warn_unused_result));

#ifdef VECTOR_TEST
#undef inline
#endif /* VECTOR_TEST */

#endif /* VECTOR_SHIFT_H */

#ifndef VECTOR_TEST
#include "shift.c"
#endif /* VECTOR_TEST */
