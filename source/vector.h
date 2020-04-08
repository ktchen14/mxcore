/**
 * @file
 *
 * A vector is a dynamic array that behaves similar to a C array. In fact a
 * vector and a C array.
 *
 * The operations available on a vector are roughly split into two interfaces:
 * an implicit interface and an explicit interface.
 *
 * Most operations in the explicit interface have a @c _z suffix and have an
 * equivalent operation in the implicit interface without that suffix.
 *
 * Implicit interface operations
 *
 * When the element type of a vector is determinate at compile the the implicit
 * interface should be used.
 *
 * Most vector operations have two forms. One with a @c _z suffix and one
 * without. The one with a @c _z suffix has an explicit @c z argument that
 * specifies the element size of the vector. The one without the suffix
 * automatically deduces the element size of the vector.
 *
 * Most operations take a vector as an argument. Unless otherwise specified in
 * the documentation of a specific operation, if the given vector is @a NULL
 * then the behavior is undefined.
 */

#ifndef VECTOR_H
#define VECTOR_H

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include "common.h"
#include "vector/common.h"

/**
 * @brief Allocate and initialize a vector by duplicating @a source
 *
 * Attempt to create a vector with the same volume and length as @a source. If
 * this fails and the length of @a source is less than its volume then attempt
 * to create a vector with the same length as @a source.
 *
 * If either of these are successful then memcpy() each element in @a source
 * into the new vector.
 *
 * If this fails then it will set @c errno to @c ENOMEM.
 *
 * @return the vector on success; otherwise @c NULL
 *
 * @see vector_duplicate() - The implicit interface analogue
 * @see vector_duplicate_z() - The explicit interface analogue
 */
vector_t vector_duplicate_z(vector_c source, size_t z)
  __attribute__((__malloc__, nonnull));

/// @copydoc vector_duplicate_z()
//= vector_t vector_duplicate(vector_c source)
#define vector_duplicate(source) \
  vector_duplicate_z(source, VECTOR_Z((source)))

/// Deallocate the @a vector and return @c NULL
void vector_delete(vector_t vector);

/// Return the volume of the @a vector
size_t vector_volume(vector_c vector) __attribute__((nonnull, pure));

/// Return the length of the @a vector
size_t vector_length(vector_c vector) __attribute__((nonnull, pure));

#include "vector/access.h"

#include "vector/create.h"
#include "vector/move.h"

#include "vector/resize.h"
#include "vector/insert.h"

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
 * @return the resultant vector
 */
vector_t vector_remove_z(vector_t vector, size_t i, size_t z)
  __attribute__((nonnull, returns_nonnull, warn_unused_result));

#define vector_remove(vector, i) \
  vector_remove_z((vector), (i), sizeof((vector)[0]))

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

/** 
 * @brief Return a pointer to the last element in the @a vector
 *
 * If no last element is in the @a vector (the @a vector's length is zero) then
 * the behavior is undefined.
 *
 * This operation is @c const qualified on the @a vector. That is if the element
 * type of the @a vector is @c const qualified (@a vector is compatible with a
 * @ref vector_c) then this will return a <tt>const void *</tt>. Otherwise this
 * will return a <tt>void *</tt>.
 */
#define vector_tail_z(vector, z) \
  vector_at((vector), vector_length((vector)) - 1, (z))

#define vector_tail(vector) vector_tail_z((vector), VECTOR_Z(vector))

/// @copydoc vector_append()
/// @see vector_append()
#define vector_push vector_append

/**
 * @brief Copy the last element in the @a vector to @a elmt and remove it
 *
 * If @a elmt is @c NULL then the element won't be copied before it's removed.
 *
 * @return the resultant vector on success; otherwise @c NULL
 */
vector_t vector_pull_z(vector_t vector, void *elmt, size_t z)
  __attribute__((nonnull(1), warn_unused_result));

#define vector_pull(vector, elmt) \
  vector_pull_z((vector), (elmt), sizeof((vector)[0]))

/// Copy the first element in the @a vector to @a elmt and remove it
vector_t vector_shift_z(vector_t vector, void *elmt, size_t z)
  __attribute__((nonnull(1), warn_unused_result));

#define vector_shift(vector, ...) \
  vector_shift_z((vector), __VA_ARGS__, VECTOR_Z((vector)))

/**
 * @brief Return whether the elements in @a a and @a b are equal according to
 *        @a eqf
 *
 * If the element type of @a a differs from the element type of @a b then the
 * behavior is undefined.
 */
bool vector_eq_z(
    vector_c a,
    vector_c b,
    int (*eqf)(const void *a, const void *b),
    size_t z)
  __attribute__((nonnull(1, 2)));

#define vector_eq(a, ...) vector_eq_z((a), __VA_ARGS__, VECTOR_Z((a)))

/// Return whether the elements in @a a and @a b differ according to @a eqf
bool vector_ne_z(vector_c a, vector_c b, eq_f eqf, size_t z);

#define vector_ne(a, b, eqf) vector_ne_z((a), (b), (eqf), sizeof((a)[0]))

/**
 * @brief Find the first element in the @a vector for which @a eqf is @c true
 *
 * @return the index of the element on success; otherwise @c ABSENT
 */
size_t vector_find_z(vector_t vector, eq_f eqf, void *data, size_t z);

#define vector_find(vector, eqf, data) \
  vector_find_z((vector), (eqf), (data), sizeof((vector)[0]))

size_t
vector_find_next_z(vector_t vector, size_t i, eq_f eqf, void *data, size_t z);

#define vector_find_next(vector, eqf, data) \
  vector_find_next_z((vector), (eqf), (data), sizeof((vector)[0]))

size_t
vector_find_last_z(vector_t vector, size_t i, eq_f eqf, void *data, size_t z);

#define vector_find_last(vector, eqf, data) \
  vector_find_last_z((vector), (eqf), (data), sizeof((vector)[0]))

size_t vector_search_z(vector_t vector, void *elmt, cmp_f cmpf, size_t z);

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
 * @return the index of the element on success; otherwise @c MX_ABSENT
 */
#define vector_search(vector, ...) \
  vector_search_z((vector), __VA_ARGS__, VECTOR_Z((vector)))

/**
 * @brief Print debugging information about the @a vector
 *
 * If @a elmt_debug is not @c NULL then it will be used to print debugging
 * information about each element in the @a vector.
 */
void vector_debug_z(
    vector_c vector, void (*elmt_debug)(const void *), size_t z);

#define vector_debug(vector, ...) \
  vector_debug_z((vector), __VA_ARGS__, VECTOR_Z((vector)))

#endif /* VECTOR_H */
