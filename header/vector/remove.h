/// @file header/vector/remove.h

#ifndef VECTOR_REMOVE_H
#define VECTOR_REMOVE_H

#ifdef VECTOR_TEST
#define inline
#endif /* VECTOR_TEST */

#include "common.h"

#include <stddef.h>

/**
 * @brief Remove the element at index @a i from the @a vector
 *
 * All elements at indices greater than @a i are shifted one element toward the
 * head of the @a vector.
 *
 * Once the element is removed and subsequent elements are shifted, if the
 * @length of the @a vector is reduced such that @f$ length \le
 * \frac{1}{2}(volume - 1) @f$, a vector_resize() will be attempted to reduce
 * the @volume to:
 *   @f[ volume = \frac{length \times 6 + 4}{5} @f]
 * On success the shrunk vector will be returned. Otherwise the vector will be
 * returned as is (without the element).
 *
 * If @a i isn't an index in the @a vector then the behavior is undefined.
 *
 * @param vector the vector to operate on
 * @param i the index in the @a vector of the element to remove
 * @return the resultant vector
 */
//= vector_t vector_remove(vector_t vector, size_t i)
#define vector_remove(v, ...) vector_remove_z((v), __VA_ARGS__, VECTOR_Z((v)))

/**
 * @brief Remove the element at index @a i from the @a vector
 *
 * All elements at indices greater than @a i are shifted one element toward the
 * head of the @a vector.
 *
 * Once the element is removed and subsequent elements are shifted, if the
 * @length of the @a vector is reduced such that @f$ length \le
 * \frac{1}{2}(volume - 1) @f$, a vector_resize_z() will be attempted to reduce
 * the @volume to:
 *   @f[ volume = \frac{length \times 6 + 4}{5} @f]
 * On success the shrunk vector will be returned. Otherwise the vector will be
 * returned as is (without the element).
 *
 * If @a i isn't an index in the @a vector then the behavior is undefined.
 *
 * @param vector the vector to operate on
 * @param i the index in the @a vector of the element to remove
 * @param z the element size of the @a vector
 * @return the resultant vector
 */
inline vector_t vector_remove_z(vector_t vector, size_t i, size_t z)
  __attribute__((nonnull, returns_nonnull, warn_unused_result));

/**
 * @brief Remove @a n elements at index @a i from the @a vector
 *
 * All elements at indices greater than <code>i + n</code> are shifted @a n
 * elements toward the head of the @a vector.
 *
 * Once the elements are removed and subsequent elements are shifted, if the
 * @length of the @a vector is reduced such that @f$ length \le
 * \frac{1}{2}(volume - 1) @f$, a vector_resize() will be attempted to reduce
 * the @volume to:
 *   @f[ volume = \frac{length \times 6 + 4}{5} @f]
 * On success the shrunk vector will be returned. Otherwise the vector will be
 * returned as is (without the elements).
 *
 * If @a i or any index from @a i to <code>i + n</code> inclusive isn't an index
 * in the @a vector then the behavior is undefined.
 *
 * @param vector the vector to operate on
 * @param i the index in the @a vector of the elements to remove
 * @param n the number of elements to remove from the @a vector
 * @return the resultant vector
 */
//= vector_t vector_excise(vector_t vector, size_t i, size_t n)
#define vector_excise(v, ...) vector_excise_z((v), __VA_ARGS__, VECTOR_Z((v)))

/**
 * @brief Remove @a n elements at index @a i from the @a vector
 *
 * All elements at indices greater than <code>i + n</code> are shifted @a n
 * elements toward the head of the @a vector.
 *
 * Once the elements are removed and subsequent elements are shifted, if the
 * @length of the @a vector is reduced such that @f$ length \le
 * \frac{1}{2}(volume - 1) @f$, a vector_resize_z() will be attempted to reduce
 * the @volume to:
 *   @f[ volume = \frac{length \times 6 + 4}{5} @f]
 * On success the shrunk vector will be returned. Otherwise the vector will be
 * returned as is (without the elements).
 *
 * If @a i or any index from @a i to <code>i + n</code> inclusive isn't an index
 * in the @a vector then the behavior is undefined.
 *
 * @param vector the vector to operate on
 * @param i the index in the @a vector of the elements to remove
 * @param n the number of elements to remove from the @a vector
 * @param z the element size of the @a vector
 * @return the resultant vector
 */
inline vector_t vector_excise_z(vector_t vector, size_t i, size_t n, size_t z)
  __attribute__((nonnull, returns_nonnull, warn_unused_result));

/**
 * @brief Reduce the @length of the @a vector to @a length
 *
 * This will remove elements from the tail of the @a vector until its length is
 * @a length. The number of elements removed is the difference between @a length
 * and the length of the @a vector. The resultant @volume of the @a vector will
 * follow the rule in vector_excise().
 *
 * If @a length is greater than the @a length of the @a vector then the behavior
 * is undefined.
 *
 * @param vector the vector to operate on
 * @param length the length of the resultant vector
 * @return the resultant vector
 */
//= vector_t vector_truncate(vector_t vector, size_t length)
#define vector_truncate(v, ...) \
  vector_truncate_z((v), __VA_ARGS__, VECTOR_Z((v)))

/**
 * @brief Reduce the @length of the @a vector to @a length
 *
 * This will remove elements from the tail of the @a vector until its length is
 * @a length. The number of elements removed is the difference between @a length
 * and the length of the @a vector. The resultant @volume of the @a vector will
 * follow the rule in vector_excise_z().
 *
 * If @a length is greater than the @a length of the @a vector then the behavior
 * is undefined.
 *
 * @param vector the vector to operate on
 * @param length the length of the resultant vector
 * @param z the element size of the @a vector
 * @return the resultant vector
 */
inline vector_t vector_truncate_z(vector_t vector, size_t length, size_t z)
  __attribute__((nonnull, returns_nonnull, warn_unused_result));

#ifdef VECTOR_TEST
#undef inline
#endif /* VECTOR_TEST */

#endif /* VECTOR_REMOVE_H */

#ifndef VECTOR_TEST
#include "remove.c"
#endif /* VECTOR_TEST */
