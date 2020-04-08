/// @file vector/access.h

#ifndef VECTOR_REMOVE_H
#define VECTOR_REMOVE_H

/// @addtogroup vector_module Vector
/// @{
/// @name Removal
/// @{

#include "common.h"

#include <stddef.h>

/**
 * @brief Remove the element at index @a i from the @a vector
 *
 * No matter what the element is first removed from the @a vector. Then if the
 * length of the @a vector is reduced such that:
 *   length <= (volume - 1) / 2
 * Then a deallocation will be attempted to reduce the volume to:
 *   volume = (length * 6 + 4) / 5
 * If this deallocation fails then the vector will be returned as is.
 *
 * @param vector the vector to operate on
 * @param i the index of the element to remove
 * @return the resultant vector
 */
//= vector_t vector_remove_z(vector_t vector, size_t i)
#define vector_remove(v, ...) vector_remove_z((v), __VA_ARGS__, VECTOR_Z((v)))

/**
 * @brief Remove the element at index @a i from the @a vector
 *
 * No matter what the element is first removed from the @a vector. Then if the
 * length of the @a vector is reduced such that:
 *   length <= (volume - 1) / 2
 * Then a deallocation will be attempted to reduce the volume to:
 *   volume = (length * 6 + 4) / 5
 * If this deallocation fails then the vector will be returned as is.
 *
 * @param vector the vector to operate on
 * @param i the index of the element to remove
 * @param z the element size of the @a vector
 * @return the resultant vector
 */
vector_t vector_remove_z(vector_t vector, size_t i, size_t z)
  __attribute__((nonnull, returns_nonnull, warn_unused_result));

/**
 * @brief Remove @a n elements at index @a i from the @a vector
 *
 * No matter what the elements are first removed from the @a vector. Then if the
 * length of the @a vector is reduced such that:
 *   length <= (volume - 1) / 2
 * Then a deallocation will be attempted to reduce the volume to:
 *   volume = (length * 6 + 4) / 5
 * If this deallocation fails then the vector will be returned as is.
 *
 * @return the resultant vector
 */
vector_t vector_excise_z(vector_t vector, size_t i, size_t n, size_t z)
  __attribute__((nonnull, returns_nonnull, warn_unused_result));

#define vector_excise(vector, i, n) \
  vector_excise_z((vector), (i), (n), VECTOR_Z((vector)))

/**
 * @brief Reduce the length of the @a vector to @a length
 *
 * No matter what elements are first removed from the tail of the @a vector
 * until its length is @a length. Then if the length of the @a vector is reduced
 * such that:
 *   @f[ length \leq \frac{volume - 1}{2} @f]
 * Then a deallocation will be attempted to reduce the volume to:
 *   @f[ volume = \frac{length \times 6 + 4}{5} @f]
 * If this deallocation fails then the vector will be returned as is.
 *
 * @return the resultant vector
 */
vector_t vector_truncate_z(vector_t vector, size_t length, size_t z)
  __attribute__((nonnull, returns_nonnull, warn_unused_result));

#define vector_truncate(vector, length) \
  vector_truncate((vector), (length), sizeof((vector)[0]))

#ifndef VECTOR_HIDE_INLINE_DEFINITION

vector_t vector_remove_z(vector_t vector, size_t i, size_t z) {
  return vector_excise_z(vector, i, 1, z);
}

vector_t vector_excise_z(vector_t vector, size_t i, size_t n, size_t z) {
  size_t length = vector_length(vector) - n;

  // move the existing elements n elements toward the head
  void *target = vector_at(vector, i + 0, z);
  void *source = vector_at(vector, i + n, z);
  size_t size = (length - i) * z;
  memmove(target, source, size);

  if (length <= (vector_volume(vector) - 1) / 2) {
    vector_t resize;
    // just volume = (length * 6 + 4) / 5 avoiding intermediate overflow
    size_t volume = length / 5 * 6 + ((length % 5) * 6 + 4) / 5;

    if ((resize = vector_resize_z(vector, volume, z)) != NULL)
      vector = resize;
  }

  // decrease the length
  __vector_to_header(vector)->length = length;

  return vector;
}

vector_t vector_truncate_z(vector_t vector, size_t length, size_t z) {
  size_t n = vector_length(vector) - length;
  return vector_excise_z(vector, vector_length(vector) - n, n, z);
}

#endif /* VECTOR_HIDE_INLINE_DEFINITION */

/// @}
/// @}

#endif /* VECTOR_REMOVE_H */
