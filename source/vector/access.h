/// @file vector/access.h

#include <stddef.h>
#include <string.h>

/// @addtogroup vector_module Vector
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
 * @see vector_set() - The inverse operation to copy data into the vector
 */
inline __attribute__((nonnull))
void vector_get(
    restrict vector_c vector,
    size_t i,
    void * restrict elmt,
    size_t z) {
  memcpy(elmt, vector_at(vector, i, z), z);
}

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
 * @see vector_set() - The inverse operation to copy data from the vector
 */
inline __attribute__((nonnull))
void vector_set(
    restrict vector_t vector,
    size_t i,
    const void * restrict elmt,
    size_t z) {
  memcpy(vector_at(vector, i, z), elmt, z);
}

/// @}
