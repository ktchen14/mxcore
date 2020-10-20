/// @file header/vector/sort.h

#ifndef VECTOR_SORT_H
#define VECTOR_SORT_H

#include <stddef.h>
#include "common.h"

#ifdef VECTOR_TEST
#define inline
#endif /* VECTOR_TEST */

/// @addtogroup vector_module Vector
/// @{
/// @name Sorting
/// @{

/**
 * @brief Sort the @a vector in ascending order on a comparator
 *
 * This isn't a stable sort: if @a cmp indicates that two elements are equal,
 * their relative order in the result is unspecified.
 *
 * @param vector the vector to operate on
 * @param cmp @parblock
 *   The comparator that will be called to establish the relative order of two
 *   elements.
 *
 *   This should return a negative integer if @a a is less than (should come
 *   before) @a b, a positive integer if @a a is greater than (should come
 *   after) @a b, and zero if @a a and @a b are equal. It must return consistent
 *   results when called for the same elements, regardless of their indices in
 *   the vector.
 *
 *   This function must encode a <b>strict total order</b> of the elements in
 *   the @a vector. That is, for any elements @c a, @c b, and <tt>c</tt>:
 *
 *   - @f$a = a@f$
 *   - If @f$a = b@f$ and @f$b = c@f$ then @f$a = c@f$
 *   - If @f$a < b@f$ then @f$b > a@f$
 *   - If @f$a < b@f$ and @f$b < c@f$ then @f$a < c@f$
 *   @endparblock
 */
//= void vector_sort(vector_t vector, int (*cmp)(const void *a, const void *b))
#define vector_sort(v, ...) vector_sort_z((v), __VA_ARGS__, VECTOR_Z((v)))

/**
 * @brief Sort the @a vector in ascending order on a comparator
 *
 * This isn't a stable sort: if @a cmp indicates that two elements are equal,
 * their relative order in the result is unspecified.
 *
 * @param vector the vector to operate on
 * @param cmp @parblock
 *   The comparator that will be called to establish the relative order of two
 *   elements.
 *
 *   This should return a negative integer if @a a is less than (should come
 *   before) @a b, a positive integer if @a a is greater than (should come
 *   after) @a b, and zero if @a a and @a b are equal. It must return consistent
 *   results when called for the same elements, regardless of their indices in
 *   the vector.
 *
 *   This function must encode a <b>strict total order</b> of the elements in
 *   the @a vector. That is, for any elements @c a, @c b, and <tt>c</tt>:
 *
 *   - @f$a = a@f$
 *   - If @f$a = b@f$ and @f$b = c@f$ then @f$a = c@f$
 *   - If @f$a < b@f$ then @f$b > a@f$
 *   - If @f$a < b@f$ and @f$b < c@f$ then @f$a < c@f$
 *   @endparblock
 * @param z the element size of the @a vector
 */
inline void vector_sort_z(
    vector_t vector,
    int (*cmp)(const void *a, const void *b),
    size_t z)
  __attribute__((nonnull));

/**
 * @brief Sort the @a vector in ascending order on a contextual comparator
 *
 * This isn't a stable sort: if @a cmp indicates that two elements are equal,
 * their relative order in the result is unspecified.
 *
 * @param vector the vector to operate on
 * @param cmp @parblock
 *   The comparator that will be called to establish the relative order of two
 *   elements.
 *
 *   This should return a negative integer if @a a is less than (should come
 *   before) @a b, a positive integer if @a a is greater than (should come
 *   after) @a b, and zero if @a a and @a b are equal. It must return consistent
 *   results when called for the same elements, regardless of their indices in
 *   the vector.
 *
 *   This function must encode a <b>strict total order</b> of the elements in
 *   the @a vector. That is, for any elements @c a, @c b, and <tt>c</tt>:
 *
 *   - @f$a = a@f$
 *   - If @f$a = b@f$ and @f$b = c@f$ then @f$a = c@f$
 *   - If @f$a < b@f$ then @f$b > a@f$
 *   - If @f$a < b@f$ and @f$b < c@f$ then @f$a < c@f$
 *   @endparblock
 * @param data contextual information to pass as the last argument to @a cmp
 */
//= void vector_sort_with(
//=     vector_t vector,
//=     int (*cmp)(const void *a, const void *b, void *data),
//=     void *data)
#define vector_sort_with(v, ...) \
  vector_sort_with_z((v), __VA_ARGS__, VECTOR_Z((v)))

/**
 * @brief Sort the @a vector in ascending order on a contextual comparator
 *
 * This isn't a stable sort: if @a cmp indicates that two elements are equal,
 * their relative order in the result is unspecified.
 *
 * @param vector the vector to operate on
 * @param cmp @parblock
 *   The comparator that will be called to establish the relative order of two
 *   elements.
 *
 *   This should return a negative integer if @a a is less than (should come
 *   before) @a b, a positive integer if @a a is greater than (should come
 *   after) @a b, and zero if @a a and @a b are equal. It must return consistent
 *   results when called for the same elements, regardless of their indices in
 *   the vector.
 *
 *   This function must encode a <b>strict total order</b> of the elements in
 *   the @a vector. That is, for any elements @c a, @c b, and <tt>c</tt>:
 *
 *   - @f$a = a@f$
 *   - If @f$a = b@f$ and @f$b = c@f$ then @f$a = c@f$
 *   - If @f$a < b@f$ then @f$b > a@f$
 *   - If @f$a < b@f$ and @f$b < c@f$ then @f$a < c@f$
 *   @endparblock
 * @param data contextual information to pass as the last argument to @a cmp
 * @param z the element size of the @a vector
 */
void vector_sort_with_z(
    vector_t vector,
    int (*cmp)(const void *a, const void *b, void *data),
    void *data,
    size_t z)
  __attribute__((nonnull(1, 2)));

#ifdef VECTOR_TEST
#undef inline
#endif /* VECTOR_TEST */

/// @}
/// @}

#endif /* VECTOR_SORT_H */

#ifndef VECTOR_TEST
#include "sort.c"
#endif /* VECTOR_TEST */
