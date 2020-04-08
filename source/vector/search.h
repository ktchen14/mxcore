/// @file vector/search.h

#ifndef VECTOR_SEARCH_H
#define VECTOR_SEARCH_H

#include <stdbool.h>

#include "common.h"

/// @addtogroup vector_module Vector
/// @{
/// @name Search
/// @{

/**
 * @brief Find the first element in the @a vector equal to @a data
 *
 * This will return the index of the first element in the @a vector for which
 * the expression <code>eqf(elmt, data)</code> is @c true (in this case @a elmt
 * is the location of an element).
 *
 * If no such element is in the @a vector then this will return @c SIZE_MAX.
 * Note that the largest possible index into an array of any type is
 * <code>SIZE_MAX - 1</code> so @c SIZE_MAX is an unambiguous indication that no
 * such element is in the @a vector.
 *
 * @param vector the vector to operate on
 * @param eqf the function to use to determine equality
 * @param data additional data to pass to @a eqf
 * @return the index of the element on success; otherwise @c SIZE_MAX
 */
//= size_t vector_find(
//=   vector_c vector,
//=   bool (*eqf)(const void *a, const void *b),
//=   const void *data)
#define vector_find(v, ...) vector_find_z((v), __VA_ARGS__, VECTOR_Z((v)))

/**
 * @brief Find the first element in the @a vector equal to @a data
 *
 * This will return the index of the first element in the @a vector for which
 * the expression <code>eqf(elmt, data)</code> is @c true (in this case @a elmt
 * is the location of an element).
 *
 * If no such element is in the @a vector then this will return @c SIZE_MAX.
 * Note that the largest possible index into an array of any type is
 * <code>SIZE_MAX - 1</code> so @c SIZE_MAX is an unambiguous indication that no
 * such element is in the @a vector.
 *
 * @param vector the vector to operate on
 * @param eqf the function to use to determine equality
 * @param data additional data to pass to @a eqf
 * @param z the element size of the @a vector
 * @return the index of the element on success; otherwise @c SIZE_MAX
 */
size_t vector_find_z(
    vector_c vector,
    bool (*eqf)(const void *a, const void *b),
    const void *data,
    size_t z)
  __attribute__((nonnull(1, 2), pure));

/**
 * @brief Find the first element at or after index @a i in the @a vector equal
 *   to @a data
 *
 * This will return the index of the first element in the @a vector at or after
 * index @a i for which the expression <code>eqf(elmt, data)</code> is @c true
 * (in this case @a elmt is the location of an element). Note that index @a i is
 * **inclusive** so if the element at that index is equal to @a data, then @a i
 * itself will be returned.
 *
 * To iterate through all elements in the @a vector equal to @a data do:
 * @code{.c}
 *   size_t i = vector_find(vector, eqf, data);
 *   while (i < SIZE_MAX) {
 *     ...
 *     i = vector_find_next(vector, eqf, i + 1, data);
 *   }
 * @endcode
 *
 * If no such element is in the @a vector at or after index @a i, then this will
 * return @c SIZE_MAX. Note that the largest possible index into an array of any
 * type is <code>SIZE_MAX - 1</code> so @c SIZE_MAX is an unambiguous indication
 * that no such element is in the @a vector.
 *
 * If @a i isn't an index in the @a vector then the behavior is undefined.
 *
 * @param vector the vector to operate on
 * @param i the lowest index in the @a vector to consider
 * @param eqf the function to call to determine if an element in the @a vector
 *   is equal to @a data
 * @param data the data
 * @return the index of the element on success; otherwise @c SIZE_MAX
 */
//= size_t vector_find_next(
//=   vector_c vector,
//=   size_t i,
//=   bool (*eqf)(const void *a, const void *b),
//=   const void *data)
#define vector_find_next(v, ...) \
  vector_find_next_z((v), __VA_ARGS__, VECTOR_Z((v)))

/**
 * @brief Find the first element at or after index @a i in the @a vector equal
 *   to @a data
 *
 * This will return the index of the first element in the @a vector at or after
 * index @a i for which the expression <code>eqf(elmt, data)</code> is @c true
 * (in this case @a elmt is the location of an element). Note that index @a i is
 * **inclusive** so if the element at that index is equal to @a data, then @a i
 * itself will be returned.
 *
 * To iterate through all elements in the @a vector equal to @a data do:
 * @code{.c}
 *   size_t i = vector_find_z(vector, eqf, data, z);
 *   while (i < SIZE_MAX) {
 *     ...
 *     i = vector_find_next_z(vector, eqf, i + 1, data, z);
 *   }
 * @endcode
 *
 * If no such element is in the @a vector at or after index @a i, then this will
 * return @c SIZE_MAX. Note that the largest possible index into an array of any
 * type is <code>SIZE_MAX - 1</code> so @c SIZE_MAX is an unambiguous indication
 * that no such element is in the @a vector.
 *
 * If @a i isn't an index in the @a vector then the behavior is undefined.
 *
 * @param vector the vector to operate on
 * @param i the lowest index in the @a vector to consider
 * @param eqf the function to use to determine equality
 * @param data additional data to pass to @a eqf
 * @param z the element size of the @a vector
 * @return the index of the element on success; otherwise @c SIZE_MAX
 */
size_t vector_find_next_z(
    vector_c vector,
    size_t i,
    bool (*eqf)(const void *a, const void *b),
    const void *data,
    size_t z)
  __attribute__((nonnull(1, 3), pure));

/**
 * @brief Find the last element before index @a i in the @a vector equal to
 *   @a data
 *
 * This will return the index of the first element in the @a vector at or after
 * index @a i for which the expression <code>eqf(elmt, data)</code> is @c true
 * (in this case @a elmt is the location of an element). Note that index @a i is
 * **inclusive** so if the element at that index is equal to @a data, then @a i
 * itself will be returned.
 *
 * To iterate in reverse order through all elements in the @a vector equal to
 * @a data do:
 * @code{.c}
 *   size_t i = vector_find_last(vector, SIZE_MAX, eqf, data);
 *   while (i < SIZE_MAX) {
 *     ...
 *     i = vector_find_last(vector, eqf, i, data);
 *   }
 * @endcode
 *
 * If no such element is in the @a vector at or after index @a i, then this will
 * return @c SIZE_MAX. Note that the largest possible index into an array of any
 * type is <code>SIZE_MAX - 1</code> so @c SIZE_MAX is an unambiguous indication
 * that no such element is in the @a vector.
 *
 * If @a i isn't an index in the @a vector then the behavior is undefined.
 */
#define vector_find_last(v, ...) \
  vector_find_last_z((v), __VA_ARGS__, VECTOR_Z((v)))

size_t
vector_find_last_z(
    vector_c vector,
    size_t i,
    bool (*eqf)(const void *a, const void *b),
    const void *data,
    size_t z)
  __attribute__((nonnull(1, 3), pure));

/**
 * @brief Find the first element in the @a vector equal to @a elmt according
 *        to @a cmpf
 *
 * The @a vector must be partitioned with respect to @a elmt according to @a
 * cmpf. That is, all the elements that compare less than must appear before all
 * the elements that compare equal to, and those must appear before all the
 * elements that compare greater than @a elmt according to @a cmpf. A vector
 * sorted by a previous call to vector_sort() with the same @a cmpf satisfies
 * these requirements.
 *
 * The behavior is undefined if the @a vector is not already partitioned with
 * respect to @a elmt in ascending order according to @a cmpf.
 *
 * @see vector_search_z() The equivalent operation in the explicit interface
 *
 * @return the index of the element on success; otherwise @c SIZE_MAX
 */
#define vector_search(v, ...) vector_search_z((v), __VA_ARGS__, VECTOR_Z((v)))

size_t vector_search_z(vector_t vector, void *elmt, cmp_f cmpf, size_t z);

#ifndef VECTOR_HIDE_INLINE_DEFINITION

inline size_t vector_find_z(
    vector_c vector,
    bool (*eqf)(const void *a, const void *b),
    const void *data,
    size_t z) {
  return vector_find_next_z(vector, 0, eqf, data, z);
}

inline size_t vector_find_next_z(
    vector_c vector,
    size_t i,
    bool (*eqf)(const void *a, const void *b),
    const void *data,
    size_t z) {
  do {
    if (eqf(vector_at(vector, i, z), data))
      return i;
  } while (++i < vector_length(vector));
  return SIZE_MAX;
}

inline size_t vector_find_last_z(
    vector_c vector,
    size_t i,
    bool (*eqf)(const void *a, const void *b),
    const void *data,
    size_t z) {
  while (i-- > 0) {
    if (eqf(vector_at(vector, i, z), data))
      return i;
  }
  return SIZE_MAX;
}

#endif /* VECTOR_HIDE_INLINE_DEFINITION */

/// @}
/// @}

#endif /* VECTOR_SEARCH_H */
