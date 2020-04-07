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

#include "vector/create.h"

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

/**
 * @brief Return a pointer to the element in the @a vector at index @a i
 *
 * @note Though this operation doesn't have the @c _z suffix, it @b is a part of
 * the explicit interface and takes the element size of the @a vector as @a z.
 * This operation is redundant if the element type of the @a vector is known at
 * compile time as it's identical to <code>vector + i</code>.
 *
 * If @a i is the length of the @a vector then this will return a pointer to
 * just past the last element of the @a vector.
 *
 * This operation is @c const qualified on the @a vector. That is if the element
 * type of the @a vector is @c const qualified (@a vector is a @ref vector_c)
 * then this will return a <tt>const void *</tt>. Otherwise this will return a
 * <tt>void *</tt>.
 *
 * This is the inverse of vector_index() such that:
 * @code{.c}
 *   vector_index(vector, vector_at(i)) == i
 *   vector_at(vector, vector_index(elmt)) == elmt
 * @endcode
 *
 * If @a i is neither an index in the @a vector or its length then the behavior
 * is undefined.
 *
 * @see vector_index() - the inverse operation to get the index of an element in
 *   a vector
 */
//= void *vector_at(vector_t vector, size_t i, size_t z)
#define vector_at(vector, i, z) ({ \
  /* Warn or fail if vector isn't a pointer. When this is absent if vector */ \
  /* is a scalar no warning is issued due to the explicit cast to char *. */ \
  const void *__vector = (vector); \
  _Pragma("GCC diagnostic push") \
  _Pragma("GCC diagnostic ignored \"-Wcast-align\"") \
  _Pragma("GCC diagnostic ignored \"-Wcast-qual\"") \
  (__typeof__((vector))) ((const char *) (__vector) + (i) * (z)); \
  _Pragma("GCC diagnostic pop") \
})

/**
 * @brief Return the index of the element at @a elmt in the @a vector
 *
 * @note Though this operation doesn't have the @c _z suffix, it @b is a part of
 * the explicit interface and takes the element size of the @a vector as @a z.
 * This operation is redundant if the element type of the @a vector is known at
 * compile time as it's identical to <code>elmt - vector</code>.
 *
 * This doesn't compare @a elmt against the elements in the @a vector; @a elmt
 * must already be a pointer to an element in the @a vector.
 *
 * This is the inverse of vector_at() such that:
 * @code{.c}
 *   vector_at(vector, vector_index(elmt)) == elmt
 *   vector_index(vector, vector_at(i)) == i
 * @endcode
 *
 * If @a elmt is @c NULL then the behavior is undefined. If @a elmt isn't a
 * pointer to an element in the @a vector then the behavior is undefined. If
 * @a elmt is a pointer to an offset in an element in the @a vector then the
 * behavior is undefined.
 *
 * @see vector_at() - The inverse operation
 */
size_t vector_index(vector_c vector, const void *elmt, size_t z)
  __attribute__((nonnull));

#include "vector/access.h"

/**
 * @brief Swap the element at index @a i with the element at index @a j in the
 * @a vector
 *
 * If either @a i or @a j isn't an index in the @a vector then the behavior of
 * this operation is undefined.
 *
 * @param vector the vector to operate on
 * @param i the index of an element in the @a vector to swap
 * @param j the index of an element in the @a vector to swap
 * @param z the element size of the @a vector
 *
 * @see vector_swap() - The implicit interface analogue
 */
void vector_swap_z(vector_t vector, size_t i, size_t j, size_t z)
  __attribute__((nonnull));

/**
 * @brief Swap the element at index @a i with the element at index @a j in the
 * @a vector
 *
 * @note Though this is implemented as a macro it's documented as a function to
 * clarify its intended usage.
 *
 * If either @a i or @a j isn't an index in the @a vector then the behavior of
 * this operation is undefined.
 *
 * @see vector_swap_z() - The explicit interface analogue
 */
//= void vector_swap(vector_t vector, size_t i, size_t j)
#define vector_swap(vector, ...) \
  vector_swap_z((vector), __VA_ARGS__, VECTOR_Z((vector)))

/**
 * @brief Move the element at index @a source to index @a target in the
 * @a vector
 *
 * This will move the element at index @a source to be at index @a target, the
 * element previously at index @a target to index <code>target + 1</code>, and
 * so on. When complete the length of the @a vector is unchanged and (with the
 * exception of the element at @a source) the relative order of each element in
 * the @a vector is unchanged. For example:
 *
 * \code{.c}
 * int *sample = vector_define(int, 2, 4, 6, 8, 10, 12);
 * vector_move(sample, 4, 2);
 * sample == { 2, 4, 10, 6, 8, 12 };
 * \endcode
 *
 * If @a target or @a source isn't an index in the @a vector then the behavior
 * is undefined.
 *
 * @param vector the vector to operate on
 * @param target the index in the @a vector to move the element to
 * @param source the index of the element in the @a vector to move
 * @param z the element size of the @a vector
 *
 * @see vector_move() - The implicit interface analogue
 */
void
vector_move_z(vector_t vector, size_t target, size_t source, size_t z)
  __attribute__((nonnull));

/**
 * @brief Move the element at index @a source to index @a target in the
 * @a vector
 *
 * This will move the element at index @a source to be at index @a target, the
 * element previously at index @a target to index <code>target + 1</code>, and
 * so on. When complete the length of the @a vector is unchanged and (with the
 * exception of the element at @a source) the relative order of each element in
 * the @a vector is unchanged. For example:
 *
 * \code{.c}
 * int *sample = vector_define(int, 2, 4, 6, 8, 10, 12);
 * vector_move(sample, 4, 2);
 * sample == { 2, 4, 10, 6, 8, 12 };
 * \endcode
 *
 * If @a target or @a source isn't an index in the @a vector then the behavior
 * is undefined.
 *
 * @param vector the vector to operate on
 * @param target the index in the @a vector to move the element to
 * @param source the index of the element in the @a vector to move
 * @param z the element size of the @a vector
 *
 * @see vector_move_z() - The explicit interface analogue
 */
//= void vector_move(vector_t vector, size_t target, size_t source)
#define vector_move(vector, target, source) \
  vector_move_z((vector), (target), (source), VECTOR_Z((vector)))

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
#define vector_resize(vector, volume) \
  vector_resize_z((vector), (volume), VECTOR_Z((vector)))

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
#define vector_shrink(vector) \
  vector_shrink_z((vector), VECTOR_Z((vector)))

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

//= vector_t vector_ensure(vector_t vector, size_t length)
#define vector_ensure(vector, length) \
  vector_ensure_z((vector), (length), sizeof((vector)[0]))

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
 * type of the @a vector is @c const qualified (@a vector is a @ref vector_c)
 * then this will return a <tt>const void *</tt>. Otherwise this will return a
 * <tt>void *</tt>.
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

/// Sort the @a vector according to @a cpmf
void vector_sort_z(vector_t vector, cmp_f cmpf, size_t z);

#define vector_sort(vector, cmpf) \
  vector_sort_z((vector), (cmpf), sizeof((vector)[0]))

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
