/**
 * @file header/vector.h
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

#include <stddef.h>

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

#include "vector/access.h"
#include "vector/comparison.h"
#include "vector/create.h"
#include "vector/debug.h"
#include "vector/delete.h"
#include "vector/insert.h"
#include "vector/move.h"
#include "vector/remove.h"
#include "vector/resize.h"
#include "vector/search.h"
#include "vector/shift.h"
#include "vector/sort.h"

#endif /* VECTOR_H */
