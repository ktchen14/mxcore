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

#ifndef MX_VECTOR_H
#define MX_VECTOR_H

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include "common.h"

/**
 * @brief Return the element size of the @a vector
 *
 * The element size is determined from the type of @a vector itself with a
 * calculation similar to <code>sizeof((vector)[0])</code>. As such to use this
 * @a vector must already be a pointer to a complete type.
 *
 * The element size of a vector isn't recorded in the vector itself. This
 * calculates the element size of the @a vector from the pointer type of the @a
 * vector itself (similar to <code>sizeof((vector)[0])</code>). Thus @a vector
 * must be a pointer to a complete type.
 *
 * This is almost identical to <code>sizeof((vector)[0])</code>. However in GNU
 * C (and similar implementations) the @c sizeof(void) is @c 1 (despite the fact
 * that @c void is an incomplete type). Because a @ref mx_vector_t is defined as
 * <code>void *</code> this raises a compiler error when @a vector is a
 * @ref mx_vector_t.
 *
 * This macro doesn't evaluate @a vector.
 */
#define MX_VECTOR_Z(vector) sizeof({ __typeof__((vector)[0]) __x; __x; })

/**
 * @brief Used to indicate a vector with an indeterminate element type
 *
 * This is intended to be used strictly when the element type of a vector is
 * indeterminate (such as when implementing this file) and shouldn't be used
 * otherwise. Instead a normal C pointer should be used. For example a vector
 * with element type @c size_t should be declared and created with:
 *
 * @code{.c}
 *   size_t *v_size = mx_vector_create();
 * @endcode
 */
typedef void * mx_vector_t;

/// A @ref mx_vector_t with a @c const element type
typedef const void * mx_vector_c;

/**
 * @brief Allocate and initialize a zero length vector
 *
 * On failure this will retain the value of @c errno set by malloc().
 *
 * @return the new vector on success; otherwise @c NULL
 */
mx_vector_t mx_vector_create(void) __attribute__((malloc));

/**
 * @brief Allocate and initialize a vector from @a length elements of @a data
 *
 * This will create and return a vector containing @a length elements from
 * @a data. On failure this will retain the value of @c errno set by malloc().
 *
 * @param data the data to initialize the vector from
 * @param length the number of elements to copy from @a data
 * @param z the element size of the @a data
 * @return the new vector on success; otherwise @c NULL
 *
 * @see mx_vector_import() - The implicit interface analogue
 * @see mx_vector_import_z() - The explicit interface analogue
 */
mx_vector_t mx_vector_import_z(const void *data, size_t length, size_t z)
  __attribute__((malloc, nonnull));

/**
 * @brief Allocate and initialize a vector from @a length elements of @a data
 *
 * This will create and return a vector containing @a length elements from
 * @a data. On failure this will retain the value of @c errno set by malloc().
 *
 * @param data the data to initialize the vector from
 * @param length the number of elements to copy from @a data
 * @return the new vector on success; otherwise @c NULL
 *
 * @see mx_vector_import() - The implicit interface analogue
 * @see mx_vector_import_z() - The explicit interface analogue
 */
//= mx_vector_t mx_vector_import(const void *data, size_t length)
#define mx_vector_import(data, length) \
  mx_vector_import_z((data), (length), MX_VECTOR_Z((data)))

/**
 * @brief Allocate and initialize a vector from the argument list
 *
 * This is just mx_vector_import() with @a data constructed and @a length
 * calculated from the argument list. If an argument in ... is incompatible with
 * @a type then the behavior is undefined.
 *
 * On failure this will retain the value of @c errno set by malloc().
 *
 * @param type a complete object type
 * @param ... a sequence of elements to initialize the vector from
 * @return the new vector on success; otherwise @c NULL
 */
#define mx_vector_define(type, ...) ({ \
  /* Fail unless type is an actual type before we declare __data with */ \
  /* __typeof__(type). Otherwise the compiler is silent on this kind of */ \
  /* mistake: */ \
  /*   mx_vector_define(1, 2, 3, 4)       => (int[])    { 2, 3, 4 } */ \
  /* When this was intended: */ \
  /*   mx_vector_define(int, 1, 2, 3, 4)  => (int[]) { 1, 2, 3, 4 } */ \
  (void) __builtin_types_compatible_p(type, void); \
  /* We must take the __typeof__(type) here so that a strange type like */ \
  /* int[2] or void (*)(void) is syntactically acceptable. */ \
  __typeof__(type) __data[] = { __VA_ARGS__ }; \
  mx_vector_import(__data, sizeof(__data) / sizeof(__data[0])); \
})

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
 * @see mx_vector_duplicate() - The implicit interface analogue
 * @see mx_vector_duplicate_z() - The explicit interface analogue
 */
mx_vector_t mx_vector_duplicate_z(mx_vector_c source, size_t z)
  __attribute__((malloc, nonnull));

/// @asimplicit{mx_vector_duplicate_z()}
//= mx_vector_t mx_vector_duplicate(mx_vector_c source)
#define mx_vector_duplicate(source) \
  mx_vector_duplicate_z(source, MX_VECTOR_Z((source)))

/// Deallocate the @a vector and return @c NULL
void mx_vector_delete(mx_vector_t vector);

/// Return the volume of the @a vector
size_t mx_vector_volume(mx_vector_c vector) __attribute__((nonnull, pure));

/// Return the length of the @a vector
size_t mx_vector_length(mx_vector_c vector) __attribute__((nonnull, pure));

/**
 * @brief Return a pointer to the element in the @a vector at index @a i
 *
 * @note Though this operation doesn't have the @c _z suffix, it @b is a part of
 * the explicit interface and takes the element size of the @a vector as @a z.
 * This operation is redundant if the element type of the @a vector is known at
 * compile time as it's identical to <code>vector + i</code>.
 *
 * This operation is @c const qualified on the @a vector. That is if the element
 * type of the @a vector is @c const qualified (@a vector is a @ref mx_vector_c)
 * then this will return a <tt>const void *</tt>. Otherwise this will return a
 * <tt>void *</tt>.
 *
 * This is the inverse of mx_vector_index() such that:
 * @code{.c}
 *   mx_vector_index(vector, mx_vector_at(i)) == i
 *   mx_vector_at(vector, mx_vector_index(elmt)) == elmt
 * @endcode
 *
 * If @a i isn't an index in the @a vector then the behavior is undefined.
 *
 * @see mx_vector_index() - The inverse operation
 */
//= void *mx_vector_at(mx_vector_t vector, size_t i, size_t z)
#define mx_vector_at(vector, i, z) ({ \
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
 * This is the inverse of mx_vector_at() such that:
 * @code{.c}
 *   mx_vector_at(vector, mx_vector_index(elmt)) == elmt
 *   mx_vector_index(vector, mx_vector_at(i)) == i
 * @endcode
 *
 * If @a elmt is @c NULL then the behavior is undefined. If @a elmt isn't a
 * pointer to an element in the @a vector then the behavior is undefined. If
 * @a elmt is a pointer to an offset in an element in the @a vector then the
 * behavior is undefined.
 *
 * @see mx_vector_at() - The inverse operation
 */
size_t mx_vector_index(mx_vector_c vector, const void *elmt, size_t z)
  __attribute__((nonnull));

/**
 * @brief Copy the element at index @a i in the @a vector into @a elmt
 *
 * @note Though this operation doesn't have the @c _z suffix, it @b is a part of
 * the explicit interface and takes the element size of the @a vector as @a z.
 * This operation is redundant if the element type of the @a vector is known at
 * compile time as it's identical to <code>*elmt = vector[i]</code>.
 *
 * If @a i isn't an index in the @a vector then the behavior is undefined. If
 * @a elmt is @c NULL then the behavior is undefined. If @a elmt is an element
 * or a location in the @a vector then the behavior is undefined.
 *
 * @param vector the vector to operate on
 * @param i the index of the element in the @a vector to copy from
 * @param elmt the location to copy the element to
 * @param z the element size of the @a vector
 *
 * @see mx_vector_set()
 */
inline __attribute__((nonnull))
void mx_vector_get(
    restrict mx_vector_c vector,
    size_t i,
    void * restrict elmt,
    size_t z) {
  memcpy(elmt, mx_vector_at(vector, i, z), z);
}

/**
 * @brief Copy the data at @a elmt into the @a vector at index @a i
 *
 * @note Though this operation doesn't have the @c _z suffix, it @b is a part of
 * the explicit interface and takes the element size of the @a vector as @a z.
 * This operation is redundant if the element type of the @a vector is known at
 * compile time as it's identical to <code>vector[i] = *elmt</code>.
 *
 * If @a i isn't an index in the @a vector then the behavior is undefined. If
 * @a elmt is @c NULL then the behavior is undefined. If @a elmt is an element
 * or a location in the @a vector then the behavior is undefined.
 *
 * @param vector the vector to operate on
 * @param i the index of the element in the @a vector to copy to
 * @param elmt the location to copy the element from
 * @param z the element size of the @a vector
 *
 * @see mx_vector_get()
 */
inline __attribute__((nonnull))
void mx_vector_set(
    restrict mx_vector_t vector,
    size_t i,
    const void * restrict elmt,
    size_t z) {
  memcpy(mx_vector_at(vector, i, z), elmt, z);
}

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
 * @see mx_vector_swap() - The implicit interface analogue
 */
void mx_vector_swap_z(mx_vector_t vector, size_t i, size_t j, size_t z)
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
 * @see mx_vector_swap_z() - The explicit interface analogue
 */
//= void mx_vector_swap(mx_vector_t vector, size_t i, size_t j)
#define mx_vector_swap(vector, ...) \
  mx_vector_swap_z((vector), __VA_ARGS__, MX_VECTOR_Z((vector)))

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
 * int *sample = mx_vector_define(int, 2, 4, 6, 8, 10, 12);
 * mx_vector_move(sample, 4, 2);
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
 * @see mx_vector_move() - The implicit interface analogue
 */
void
mx_vector_move_z(mx_vector_t vector, size_t target, size_t source, size_t z)
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
 * int *sample = mx_vector_define(int, 2, 4, 6, 8, 10, 12);
 * mx_vector_move(sample, 4, 2);
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
 * @see mx_vector_move_z() - The explicit interface analogue
 */
//= void mx_vector_move(mx_vector_t vector, size_t target, size_t source)
#define mx_vector_move(vector, target, source) \
  mx_vector_move_z((vector), (target), (source), MX_VECTOR_Z((vector)))

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
mx_vector_t mx_vector_resize_z(mx_vector_t vector, size_t volume, size_t z)
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
//= mx_vector_t mx_vector_resize(mx_vector_t vector, size_t volume)
#define mx_vector_resize(vector, volume) \
  mx_vector_resize_z((vector), (volume), MX_VECTOR_Z((vector)))

/**
 * @brief Resize the volume of the @a vector to its length
 *
 * This is similar to <code>mx_vector_resize_z(vector, mx_vector_length(vector),
 * z)</code>. However if the mx_vector_resize_z() fails then the @a vector will
 * be returned umodified.
 *
 * @return the shrunk vector on success; otherwise the unmodified vector
 *
 * @see mx_vector_shrink()
 */
mx_vector_t mx_vector_shrink_z(mx_vector_t vector, size_t z)
  __attribute__((nonnull, returns_nonnull, warn_unused_result));

/**
 * @brief Resize the volume of the @a vector to its length
 *
 * This is similar to <code>mx_vector_resize(vector,
 * mx_vector_length(vector))</code>. However if the mx_vector_resize() fails
 * then the @a vector will be returned umodified.
 *
 * @return the shrunk vector on success; otherwise the unmodified vector
 *
 * @see mx_vector_shrink_z()
 */
//= mx_vector_t mx_vector_shrink(mx_vector_t vector)
#define mx_vector_shrink(vector) \
  mx_vector_shrink_z((vector), sizeof((vector)[0]))

/**
 * @brief Ensure that the volume of the @a vector is at least @a length
 *
 * If the volume of the @a vector is less than @a length then mx_vector_resize()
 * will be called. Preallocation will first be attempted in order to accomodate
 * further increases in length according to the formula:
 *   volume = (length * 8 + 3) / 5
 * If this preallocation fails then a resize to @a length will be attempted. If
 * this also fails then the @a vector will be unmodified.
 *
 * After a successful mx_vector_ensure() subsequent mx_vector_insert()s (and
 * mx_vector_append()s) into the vector are guaranteed to be successful as long
 * as the resultant length doesn't exceed @a length.
 *
 * Note that the @a vector does not remember this @a length. As a result calling
 * any functions that can decrease the volume of the @a vector such as:
 *   mx_vector_resize()
 *   mx_vector_shrink()
 *   mx_vector_remove()
 * Will invalidate this guarantee.
 *
 * @return the resultant vector on success; otherwise @c NULL
 */
mx_vector_t mx_vector_ensure_z(mx_vector_t vector, size_t length, size_t z)
  __attribute__((nonnull, warn_unused_result));

//= mx_vector_t mx_vector_ensure(mx_vector_t vector, size_t length)
#define mx_vector_ensure(vector, length) \
  mx_vector_ensure_z((vector), (length), sizeof((vector)[0]))

/// @copybrief mx_vector_insert()
/// @see mx_vector_insert()
mx_vector_t
mx_vector_insert_z(mx_vector_t vector, size_t i, const void *elmt, size_t z)
  __attribute__((nonnull(1), warn_unused_result));

/**
 * @brief Insert the data at @a elmt into the @a vector at index @a i
 *
 * If @a elmt is @c NULL then the inserted element will be uninitialized.
 *
 * This will call mx_vector_ensure(). If that fails then the @a vector will be
 * unmodified.
 *
 * If this fails then it will set @c errno to @c ENOMEM.
 *
 * If @a i isn't an index in the @a vector or the length of the @a vector then
 * the behavior is undefined. If @a elmt isn't @c NULL and its type isn't
 * compatible with the element type of the @a vector then the behavior is
 * undefined.
 *
 * @return the resultant vector on success; otherwise @c NULL
 */
//= mx_vector_t mx_vector_insert(mx_vector_t vector, size_t i, void *elmt)
#define mx_vector_insert(vector, i, elmt) \
  mx_vector_insert_z((vector), (i), (elmt), MX_VECTOR_Z((vector)))

/// @copydoc mx_vector_inject()
/// @see mx_vector_inject()
mx_vector_t mx_vector_inject_z(
    mx_vector_t vector, size_t i, const void *elmt, size_t n, size_t z)
  __attribute__((nonnull(1), warn_unused_result));

/**
 * @brief Insert @a n elements from @a elmt into the @a vector starting at index
 *        @a i
 *
 * If @a elmt is @c NULL then the inserted elements will be uninitialized.
 *
 * This will call mx_vector_ensure(). If that fails then the @a vector will be
 * unmodified.
 *
 * This is more efficient than calling mx_vector_insert() @a n times as the
 * elements after <code>i + n</code> will be shifted only once.
 *
 * If this fails then it will set @c errno to @c ENOMEM.
 *
 * If @a i isn't an index in the @a vector or the length of the @a vector then
 * the behavior is undefined.
 *
 * @return the resultant vector on success; otherwise @c NULL
 */
#define mx_vector_inject(vector, i, elmt, n) \
  mx_vector_inject_z((vector), (i), (elmt), (n), MX_VECTOR_Z((vector)))

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
mx_vector_t mx_vector_remove_z(mx_vector_t vector, size_t i, size_t z)
  __attribute__((nonnull, returns_nonnull, warn_unused_result));

#define mx_vector_remove(vector, i) \
  mx_vector_remove_z((vector), (i), sizeof((vector)[0]))

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
mx_vector_t mx_vector_excise_z(mx_vector_t vector, size_t i, size_t n, size_t z)
  __attribute__((nonnull, returns_nonnull, warn_unused_result));

#define mx_vector_excise(vector, i, n) \
  mx_vector_excise((vector), (i), (n), sizeof((vector)[0]))

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
mx_vector_t mx_vector_truncate_z(mx_vector_t vector, size_t length, size_t z)
  __attribute__((nonnull, returns_nonnull, warn_unused_result));

#define mx_vector_truncate(vector, length) \
  mx_vector_truncate((vector), (length), sizeof((vector)[0]))

/// @copydoc mx_vector_append()
/// @see mx_vector_append()
mx_vector_t mx_vector_append_z(mx_vector_t vector, const void *elmt, size_t z)
  __attribute__((nonnull(1), warn_unused_result));

/**
 * @brief Insert the data at @a elmt as the last element in the @a vector
 *
 * If @a elmt is @c NULL then the appended element will be uninitialized.
 *
 * This will call mx_vector_ensure() or its explicit interface equivalent. If
 * that fails then the @a vector will be unmodified.
 *
 * If this fails then it will set @c errno to @c ENOMEM.
 *
 * @return the resultant vector on success; otherwise @c NULL
 *
 * @see mx_vector_insert() - To insert the element at a specified index
 * @see mx_vector_append_z() - The explicit interface equivalent of this
 * operation
 */
#define mx_vector_append(vector, elmt) \
  mx_vector_append_z((vector), (elmt), sizeof((vector)[0]))

/**
 * @brief Append @a n elements from @a elmt to the @a vector
 *
 * If @a elmt is @c NULL then random data will inserted.
 *
 * This will call mx_vector_ensure(). If that fails then the @a vector will be
 * unmodified.
 *
 * @return the resultant vector on success; otherwise @c NULL
 */
mx_vector_t
mx_vector_extend_z(mx_vector_t vector, const void *elmt, size_t n, size_t z)
  __attribute__((nonnull(1), warn_unused_result));

#define mx_vector_extend(vector, elmt, n) \
  mx_vector_extend_z((vector), (elmt), (n), sizeof((vector)[0]))

/** 
 * @brief Return a pointer to the last element in the @a vector
 *
 * If no last element is in the @a vector (the @a vector's length is zero) then
 * the behavior is undefined.
 *
 * This operation is @c const qualified on the @a vector. That is if the element
 * type of the @a vector is @c const qualified (@a vector is a @ref mx_vector_c)
 * then this will return a <tt>const void *</tt>. Otherwise this will return a
 * <tt>void *</tt>.
 */
#define mx_vector_tail_z(vector, z) \
  mx_vector_at((vector), mx_vector_length((vector)) - 1, (z))

#define mx_vector_tail(vector) mx_vector_tail_z((vector), MX_VECTOR_Z(vector))

/// @copydoc mx_vector_append()
/// @see mx_vector_append()
#define mx_vector_push mx_vector_append

/**
 * @brief Copy the last element in the @a vector to @a elmt and remove it
 *
 * If @a elmt is @c NULL then the element won't be copied before it's removed.
 *
 * @return the resultant vector on success; otherwise @c NULL
 */
mx_vector_t mx_vector_pull_z(mx_vector_t vector, void *elmt, size_t z)
  __attribute__((nonnull(1), warn_unused_result));

#define mx_vector_pull(vector, elmt) \
  mx_vector_pull_z((vector), (elmt), sizeof((vector)[0]))

/// Copy the first element in the @a vector to @a elmt and remove it
mx_vector_t mx_vector_shift_z(mx_vector_t vector, void *elmt, size_t z)
  __attribute__((nonnull(1), warn_unused_result));

#define mx_vector_shift(vector, ...) \
  mx_vector_shift_z((vector), __VA_ARGS__, MX_VECTOR_Z((vector)))

/**
 * @brief Return whether the elements in @a a and @a b are equal according to
 *        @a eqf
 *
 * If the element type of @a a differs from the element type of @a b then the
 * behavior is undefined.
 */
bool mx_vector_eq_z(
    mx_vector_c a,
    mx_vector_c b,
    int (*eqf)(const void *a, const void *b),
    size_t z)
  __attribute__((nonnull(1, 2)));

#define mx_vector_eq(a, ...) mx_vector_eq_z((a), __VA_ARGS__, MX_VECTOR_Z((a)))

/// Return whether the elements in @a a and @a b differ according to @a eqf
bool mx_vector_ne_z(mx_vector_c a, mx_vector_c b, mx_eq_f eqf, size_t z);

#define mx_vector_ne(a, b, eqf) mx_vector_ne_z((a), (b), (eqf), sizeof((a)[0]))

/// Sort the @a vector according to @a cpmf
void mx_vector_sort_z(mx_vector_t vector, mx_cmp_f cmpf, size_t z);

#define mx_vector_sort(vector, cmpf) \
  mx_vector_sort_z((vector), (cmpf), sizeof((vector)[0]))

/**
 * @brief Find the first element in the @a vector for which @a eqf is @c true
 *
 * @return the index of the element on success; otherwise @c MX_ABSENT
 */
size_t mx_vector_find_z(mx_vector_t vector, mx_eq_f eqf, void *data, size_t z);

#define mx_vector_find(vector, eqf, data) \
  mx_vector_find_z((vector), (eqf), (data), sizeof((vector)[0]))

size_t
mx_vector_find_next_z(mx_vector_t vector, size_t i, mx_eq_f eqf, void *data, size_t z);

#define mx_vector_find_next(vector, eqf, data) \
  mx_vector_find_next_z((vector), (eqf), (data), sizeof((vector)[0]))

size_t
mx_vector_find_last_z(mx_vector_t vector, size_t i, mx_eq_f eqf, void *data, size_t z);

#define mx_vector_find_last(vector, eqf, data) \
  mx_vector_find_last_z((vector), (eqf), (data), sizeof((vector)[0]))

size_t mx_vector_search_z(mx_vector_t vector, void *elmt, mx_cmp_f cmpf, size_t z);

/**
 * @brief Find the first element in the @a vector equal to @a elmt according
 *        to @a cmpf
 *
 * The @a vector must be partitioned with respect to @a elmt according to @a
 * cmpf. That is, all the elements that compare less than must appear before all
 * the elements that compare equal to, and those must appear before all the
 * elements that compare greater than @a elmt according to @a cmpf. A vector
 * sorted by a previous call to mx_vector_sort() with the same @a cmpf satisfies
 * these requirements.
 *
 * The behavior is undefined if the @a vector is not already partitioned with
 * respect to @a elmt in ascending order according to @a cmpf.
 *
 * @see mx_vector_search_z() The equivalent operation in the explicit interface
 *
 * @return the index of the element on success; otherwise @c MX_ABSENT
 */
#define mx_vector_search(vector, ...) \
  mx_vector_search_z((vector), __VA_ARGS__, MX_VECTOR_Z((vector)))

/**
 * @brief Print debugging information about the @a vector
 *
 * If @a elmt_debug is not @c NULL then it will be used to print debugging
 * information about each element in the @a vector.
 */
void mx_vector_debug_z(
    mx_vector_c vector, void (*elmt_debug)(const void *), size_t z);

#define mx_vector_debug(vector, ...) \
  mx_vector_debug_z((vector), __VA_ARGS__, MX_VECTOR_Z((vector)))

#endif /* MX_VECTOR_H */
