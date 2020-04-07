/// @file vector/access.h

#ifndef VECTOR_ACCESS_H
#define VECTOR_ACCESS_H

#include "common.h"

#include <stddef.h>
#include <string.h>

/// @addtogroup vector_module Vector
/// @{
/// @name Access
/// @{

/**
 * @brief Copy the element at index @a i in the @a vector into @a elmt
 *
 * @note Though this operation doesn't have the @c _z suffix, it @b is a part of
 * the explicit interface and takes the element size of the @a vector as @a z.
 * This operation is redundant if the element type of the @a vector is known at
 * compile time as it's identical to <code>*elmt = vector[i]</code>.
 *
 * If @a i isn't an index in the @a vector, @a elmt is @c NULL, or @a elmt
 * overlaps the element at index @a i in the @a vector, then the behavior is
 * undefined.
 *
 * @param vector the vector to operate on
 * @param i the index of the element in the @a vector to copy from
 * @param elmt the location to copy the element to
 * @param z the element size of the @a vector
 *
 * @see vector_set() - the inverse operation to copy data into a vector
 */
void vector_get(
    restrict vector_c vector,
    size_t i,
    void *restrict elmt,
    size_t z)
  __attribute__((nonnull));

/**
 * @brief Copy the data at @a elmt into the @a vector at index @a i
 *
 * @note Though this operation doesn't have the @c _z suffix, it @b is a part of
 * the explicit interface and takes the element size of the @a vector as @a z.
 * This operation is redundant if the element type of the @a vector is known at
 * compile time as it's identical to <code>vector[i] = *elmt</code>.
 *
 * If @a i isn't an index in the @a vector, @a elmt is @c NULL, or @a elmt
 * overlaps the element at index @a i in the @a vector, then the behavior is
 * undefined.
 *
 * @param vector the vector to operate on
 * @param i the index of the element in the @a vector to copy to
 * @param elmt the location to copy the element from
 * @param z the element size of the @a vector
 *
 * @see vector_get() - the inverse operation to copy data from a vector
 */
void vector_set(
    restrict vector_t vector,
    size_t i,
    const void *restrict elmt,
    size_t z)
  __attribute__((nonnull));

#ifndef VECTOR_HIDE_INLINE_DEFINITION

inline void vector_get(
    restrict vector_c vector,
    size_t i,
    void *restrict elmt,
    size_t z) {
  memcpy(elmt, vector_at(vector, i, z), z);
}

inline void vector_set(
    restrict vector_t vector,
    size_t i,
    const void *restrict elmt,
    size_t z) {
  memcpy(vector_at(vector, i, z), elmt, z);
}

#endif /* VECTOR_HIDE_INLINE_DEFINITION */

/// @}
/// @}

#endif /* VECTOR_ACCESS_H */
