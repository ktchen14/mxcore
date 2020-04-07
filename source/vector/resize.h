/// @file vector/resize.h

#ifndef VECTOR_RESIZE_H
#define VECTOR_RESIZE_H

#include "common.h"

/// @addtogroup vector_module Vector
/// @{
/// @name Resize
/// @{

/**
 * @brief Resize the volume of the @a vector to @a volume
 *
 * This will fail if realloc() returns @c NULL. The C standard <b>doesn't</b>
 * guarantee that realloc() to a smaller size will be successful so this can
 * fail even if the requested @a volume is less than the current volume of the
 * @a vector. If the realloc() fails then the @a vector will be unmodified.
 *
 * If @a volume is less than the length of the @a vector then the @a vector will
 * be truncated and have its length reduced to @a volume.
 *
 * If this fails then it will set @c errno to @c ENOMEM.
 *
 * @param vector the vector to operate on
 * @param volume the volume to resize the @a vector to
 * @return the resized vector on success; otherwise @c NULL
 */
//= vector_t vector_resize(vector_t vector, size_t volume)
#define vector_resize(v, ...) vector_resize_z((v), __VA_ARGS__, VECTOR_Z((v)))

/**
 * @brief Resize the volume of the @a vector to @a volume
 *
 * This will fail if realloc() returns @c NULL. The C standard <b>doesn't</b>
 * guarantee that realloc() to a smaller size will be successful so this can
 * fail even if the requested @a volume is less than the current volume of the
 * @a vector. If the realloc() fails then the @a vector will be unmodified.
 *
 * If @a volume is less than the length of the @a vector then the @a vector will
 * be truncated and have its length reduced to @a volume.
 *
 * If this fails then it will set @c errno to @c ENOMEM.
 *
 * @param vector the vector to operate on
 * @param volume the volume to resize the @a vector to
 * @param z the element size of the @a vector
 * @return the resized vector on success; otherwise @c NULL
 */
vector_t vector_resize_z(vector_t vector, size_t volume, size_t z)
  __attribute__((nonnull, warn_unused_result));

/**
 * @brief Reduce the volume of the @a vector to its length
 *
 * This is similar to a vector_resize() to the vector's length. However if
 * the resize fails then the @a vector will be returned umodified and the value
 * of @c errno set by realloc() will be retained.
 *
 * @param vector the vector to operate on
 * @return the shrunk vector on success; otherwise the unmodified vector
 *
 * @see vector_shrink_z() - the explicit interface analogue
 */
//= vector_t vector_shrink(vector_t vector)
#define vector_shrink(v) vector_shrink_z((v), VECTOR_Z((v)))

/**
 * @brief Reduce the volume of the @a vector to its length
 *
 * This is similar to a vector_resize_z() to the vector's length. However if
 * the resize fails then the @a vector will be returned umodified and the value
 * of @c errno set by realloc() will be retained.
 *
 * @param vector the vector to operate on
 * @param z the element size of the @a vector
 * @return the shrunk vector on success; otherwise the unmodified vector
 *
 * @see vector_shrink() - the implicit interface analogue
 */
vector_t vector_shrink_z(vector_t vector, size_t z)
  __attribute__((nonnull, returns_nonnull, warn_unused_result));

//= vector_t vector_ensure(vector_t vector, size_t length)
#define vector_ensure(v, ...) vector_ensure_z((v), __VA_ARGS__, VECTOR_Z((v)))

/**
 * @brief Ensure that the volume of the @a vector is at least @a length
 *
 * If the volume of the @a vector is less than @a length then vector_resize()
 * will be called. Preallocation will first be attempted in order to accomodate
 * further increases in length according to the formula:
 *   volume = (length * 8 + 3) / 5
 * If this preallocation fails then a resize to @a length will be attempted. If
 * this also fails then the @a vector will be unmodified.
 *
 * After a successful vector_ensure() subsequent vector_insert()s (and
 * vector_append()s) into the vector are guaranteed to be successful as long
 * as the resultant length doesn't exceed @a length.
 *
 * Note that the @a vector does not remember this @a length. As a result calling
 * any functions that can decrease the volume of the @a vector such as:
 *   vector_resize()
 *   vector_shrink()
 *   vector_remove()
 * Will invalidate this guarantee.
 *
 * @return the resultant vector on success; otherwise @c NULL
 */
vector_t vector_ensure_z(vector_t vector, size_t length, size_t z)
  __attribute__((nonnull, warn_unused_result));

#ifndef VECTOR_HIDE_INLINE_DEFINITION

inline vector_t vector_resize_z(vector_t vector, size_t volume, size_t z) {
  header_t *header = vector_to_header(vector);
  size_t size;

  // calculate size and test for overflow
  if (__builtin_mul_overflow(volume, z, &size))
    return errno = ENOMEM, NULL;
  if (__builtin_add_overflow(size, sizeof(header_t), &size))
    return errno = ENOMEM, NULL;

  if ((header = realloc(header, size)) == NULL)
    return NULL;

  if ((header->volume = volume) < header->length)
    header->length = volume;

  return header_to_vector(header);
}

inline vector_t vector_shrink_z(vector_t vector, size_t z) {
  vector_t shrunk;

  shrunk = vector_resize_z(vector, vector_length(vector), z);

  return shrunk != NULL ? shrunk : vector;
}

inline vector_t vector_ensure_z(vector_t vector, size_t length, size_t z) {
  if (length <= vector_volume(vector))
    return vector;

  // just volume = (length * 8 + 3) / 5 avoiding intermediate overflow
  size_t volume = length / 5 * 8 + ((length % 5) * 8 + 3) / 5;

  // if the volume doesn't overflow then attempt to allocate it
  if (volume > length) {
    vector_t resize;
    if ((resize = vector_resize_z(vector, volume, z)) != NULL)
      return resize;
  }

  // if either the volume overflows or the allocation failed then attempt to
  // resize to just the length
  return vector_resize_z(vector, length, z);
}

#endif /* VECTOR_HIDE_INLINE_DEFINITION */

/// @}
/// @}

#endif /* VECTOR_ACCESS_H */
