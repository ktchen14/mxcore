/// @file header/vector/resize.h

#ifndef VECTOR_RESIZE_H
#define VECTOR_RESIZE_H

#include <stddef.h>
#include "common.h"

#ifdef VECTOR_TEST
#define inline
#endif /* VECTOR_TEST */

/**
 * @brief Resize the @volume of the @a vector to @a volume
 *
 * If @a volume is less than the @length of the @a vector then the @a vector
 * will be truncated and have its length reduced to @a volume.
 *
 * This will fail if the underlying realloc() returns @c NULL. The C standard
 * <b>doesn't</b> guarantee success of a realloc() to a smaller size, so this
 * can fail even if @a volume is less than the volume of the @a vector. In
 * either case if the realloc() fails then the @a vector will be unmodified and
 * the value of @c errno set by realloc() will be retained.
 *
 * @param vector the vector to operate on
 * @param volume the volume to resize the @a vector to
 * @return the resized vector on success; otherwise @c NULL
 *
 * @see vector_resize_z() - the explicit interface analogue
 */
//= vector_t vector_resize(vector_t vector, size_t volume)
#define vector_resize(v, ...) vector_resize_z((v), __VA_ARGS__, VECTOR_Z((v)))

/**
 * @brief Resize the @volume of the @a vector to @a volume
 *
 * If @a volume is less than the @length of the @a vector then the @a vector
 * will be truncated and have its length reduced to @a volume.
 *
 * This will fail if the underlying realloc() returns @c NULL. The C standard
 * <b>doesn't</b> guarantee success of a realloc() to a smaller size, so this
 * can fail even if @a volume is less than the volume of the @a vector. In
 * either case if the realloc() fails then the @a vector will be unmodified and
 * the value of @c errno set by realloc() will be retained.
 *
 * @param vector the vector to operate on
 * @param volume the volume to resize the @a vector to
 * @param z the element size of the @a vector
 * @return the resized vector on success; otherwise @c NULL
 *
 * @see vector_resize() - the implicit interface analogue
 */
inline vector_t vector_resize_z(vector_t vector, size_t volume, size_t z)
  __attribute__((nonnull, warn_unused_result));

/**
 * @brief Ensure that the @volume of the @a vector is no less than @a length
 *
 * If the volume of the @a vector is less than @a length then vector_resize()
 * will be called. Preallocation is attempted to accomodate future increases in
 * @length according to the formula:
 *   @f[ volume = \frac{length \times 8 + 3}{5} @f]
 * If this preallocation fails then a resize to @a length will be attempted. If
 * that also fails then the @a vector will be unmodified.
 *
 * On success, subsequent insertions (through vector_insert(), vector_append(),
 * etc.) into the vector are guaranteed to be successful so long as the
 * resultant length doesn't exceed @a length. Note that the @a vector doesn't
 * remember this @a length. As a result calling any functions that can decrease
 * the volume of the @a vector (like vector_resize(), vector_shrink(),
 * vector_remove(), etc.) will invalidate this guarantee.
 *
 * @param vector the vector to operate on
 * @param length the minimum length to accomodate in the @a vector
 * @return the resultant vector on success; otherwise @c NULL
 */
//= vector_t vector_ensure(vector_t vector, size_t length)
#define vector_ensure(v, ...) vector_ensure_z((v), __VA_ARGS__, VECTOR_Z((v)))

/**
 * @brief Ensure that the @volume of the @a vector is at least @a length
 *
 * If the volume of the @a vector is less than @a length then vector_resize_z()
 * will be called. Preallocation is attempted to accomodate future increases in
 * @length according to the formula:
 *   @f[ volume = \frac{length \times 8 + 3}{5} @f]
 * If this preallocation fails then a resize to @a length will be attempted. If
 * that also fails then the @a vector will be unmodified.
 *
 * On success, subsequent insertions (through vector_insert_z(),
 * vector_append_z(), etc.) into the vector are guaranteed to be successful so
 * long as the resultant length doesn't exceed @a length. Note that the
 * @a vector doesn't remember this @a length. As a result calling any functions
 * that can decrease the volume of the @a vector (like vector_resize_z(),
 * vector_shrink_z(), vector_remove_z(), etc.) will invalidate this guarantee.
 *
 * @param vector the vector to operate on
 * @param length the minimum length to accomodate in the @a vector
 * @param z the element size of the @a vector
 * @return the resultant vector on success; otherwise @c NULL
 */
inline vector_t vector_ensure_z(vector_t vector, size_t length, size_t z)
  __attribute__((nonnull, warn_unused_result));

/**
 * @brief Reduce the volume of the @a vector to its length
 *
 * This is almost a vector_resize() to the vector's length. However if the
 * resize fails then @a vector will be returned umodified and the value of
 * @c errno set by realloc() will be retained.
 *
 * @param vector the vector to operate on
 * @return the shrunk vector on success; otherwise the unmodified @a vector
 *
 * @see vector_shrink_z() - the explicit interface analogue
 */
//= vector_t vector_shrink(vector_t vector)
#define vector_shrink(v) vector_shrink_z((v), VECTOR_Z((v)))

/**
 * @brief Reduce the volume of the @a vector to its length
 *
 * This is almost a vector_resize_z() to the vector's length. However if the
 * resize fails then the @a vector will be returned umodified and the value of
 * @c errno set by realloc() will be retained.
 *
 * @param vector the vector to operate on
 * @param z the element size of the @a vector
 * @return the shrunk vector on success; otherwise the unmodified @a vector
 *
 * @see vector_shrink() - the implicit interface analogue
 */
inline vector_t vector_shrink_z(vector_t vector, size_t z)
  __attribute__((nonnull, returns_nonnull, warn_unused_result));

#ifdef VECTOR_TEST
#undef inline
#endif /* VECTOR_TEST */

#endif /* VECTOR_RESIZE_H */

#ifndef VECTOR_TEST
#include "resize.c"
#endif /* VECTOR_TEST */
