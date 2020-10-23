/// @file header/vector/common.h

#ifndef VECTOR_COMMON_H
#define VECTOR_COMMON_H

#include <stddef.h>

/**
 * @brief Used to indicate a vector with an indeterminate element type
 *
 * This is intended to be used strictly when the element type of a vector is
 * indeterminate (such as when implementing this file) and shouldn't be used
 * otherwise. Instead a normal C pointer should be used. For example a vector
 * with element type @c size_t should be declared and created with:
 *
 * @code{.c}
 *   size_t *v_size = vector_create();
 * @endcode
 */
typedef void * vector_t;

/// A @ref vector_t with a @c const element type
typedef void const * vector_c;

/**
 * @brief Return the volume of the @a vector
 *
 * A vector's volume is the number of elements that it can hold without a
 * reallocation and should always be greater than or equal to the vector's
 * @length.
 */
inline size_t vector_volume(vector_c vector) __attribute__((nonnull, pure));

/// Return the length of (the number of elements in) the @a vector
inline size_t vector_length(vector_c vector) __attribute__((nonnull, pure));

/// @cond INTERNAL

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
 * that @c void is an incomplete type). Because a @ref vector_t is defined as
 * <code>void *</code> this raises a compiler error when @a vector is a
 * @ref vector_t.
 *
 * This macro doesn't evaluate @a vector.
 */
#define VECTOR_Z(vector) sizeof({ __typeof__((vector)[0]) __x; __x; })

/// The header of a @ref vector_t or @ref vector_c
struct __vector_header_t {
  size_t volume;
  size_t length;
  _Alignas(max_align_t) char data[];
};

/**
 * @brief Return the header associated with the @a vector
 *
 * This function is @a const qualified on the @a vector. That is if the element
 * type of the @a vector is @c const qualified (@a vector is compatible with a
 * @ref vector_c), this will return a <tt>const struct __vector_header_t *</tt>.
 * Otherwise this will return a <tt>struct __vector_header_t *</tt>.
 */
#define __vector_to_header(vector) ({ \
  _Pragma("GCC diagnostic push"); \
  _Pragma("GCC diagnostic ignored \"-Wcast-align\""); \
  _Pragma("GCC diagnostic ignored \"-Wcast-qual\""); \
  _Generic((vector), vector_t: (struct __vector_header_t *) ( \
      (/* */ char *) (vector) - offsetof(struct __vector_header_t, data) \
    ), vector_c: (const struct __vector_header_t *) ( \
      (const char *) (vector) - offsetof(struct __vector_header_t, data) \
    )); \
  _Pragma("GCC diagnostic pop") \
})

/// @endcond

inline size_t vector_volume(vector_c vector) {
  return __vector_to_header(vector)->volume;
}

inline size_t vector_length(vector_c vector) {
  return __vector_to_header(vector)->length;
}

#endif /* VECTOR_COMMON_H */

#ifndef VECTOR_TEST
#include "common.c"
#endif /* VECTOR_TEST */
