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

#include "vector/common.h"

/**
 * @brief The equality function type to return whether @a a and @a b are
 *        equivalent
 *
 * An function of this type must return @c 0 if @a a and @a b aren't equivalent
 * and any @c int other than @c 0 if @a a and @a b are equivalent.
 */
typedef int (*eq_f)(const void *a, const void *b);

typedef int (*cmp_f)(const void *a, const void *b);

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

#include "vector/access.h"

#include "vector/create.h"
#include "vector/move.h"

#include "vector/resize.h"
#include "vector/insert.h"
#include "vector/remove.h"

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

#include "vector/search.h"

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
