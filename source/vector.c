/// @file vector.c

#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "vector.h"

typedef struct _header_t {
  size_t volume;
  size_t length;
  char data[] __attribute__((aligned));
} header_t;

/// Return the vector associated with the @a header
static inline mx_vector_t header_to_vector(header_t *header)
  __attribute__((const, nonnull, returns_nonnull));

/**
 * @brief Return the header associated with the @a vector
 *
 * This function is @a const qualified on the @a vector. That is if the element
 * type of the @a vector is @c const qualified (@a vector is a @c mx_vector_c)
 * then this will return a <tt>const header_t *</tt>. Otherwise this will return
 * a <tt>header_t *</tt>.
 */
#define vector_to_header(vector) ({ \
  _Pragma("GCC diagnostic push"); \
  _Pragma("GCC diagnostic ignored \"-Wcast-align\""); \
  _Pragma("GCC diagnostic ignored \"-Wcast-qual\""); \
  _Generic((vector), mx_vector_t: (/* */ header_t *) ( \
      (/* */ char *) (vector) - offsetof(header_t, data) \
    ), mx_vector_c: (const header_t *) ( \
      (const char *) (vector) - offsetof(header_t, data) \
    )); \
  _Pragma("GCC diagnostic pop") \
})

mx_vector_t mx_vector_create() {
  header_t *header;

  if ((header = malloc(sizeof(header_t))) == NULL)
    return NULL;

  header->volume = 0;
  header->length = 0;

  return header_to_vector(header);
}

mx_vector_t mx_vector_import_z(const void *data, size_t length, size_t z) {
  header_t *header;

  // Doesn't overflow because this is the size of data
  size_t size = length * z;
  if (__builtin_add_overflow(size, sizeof(header_t), &size))
    return errno = ENOMEM, NULL;
  if ((header = malloc(size)) == NULL)
    return NULL;

  header->volume = length;
  header->length = length;
  memcpy(header->data, data, length * z);

  return header_to_vector(header);
}

mx_vector_t mx_vector_duplicate_z(mx_vector_c source, size_t z) {
  header_t *header;

  size_t volume = mx_vector_volume(source);
  size_t length = mx_vector_length(source);

  if ((header = malloc(sizeof(header_t) + volume * z)) == NULL) {
    if (length == volume)
      return NULL;
    if ((header = malloc(sizeof(header_t) + length * z)) == NULL)
      return NULL;
    header->volume = length;
  } else
    header->volume = volume;

  header->length = length;

  memcpy(header->data, source, length * z);

  return header_to_vector(header);
}

void mx_vector_delete(mx_vector_t vector) {
  free(vector_to_header(vector));
}

size_t mx_vector_volume(mx_vector_c vector) {
  return vector_to_header(vector)->volume;
}

size_t mx_vector_length(mx_vector_c vector) {
  return vector_to_header(vector)->length;
}

size_t mx_vector_index(mx_vector_c vector, const void *elmt, size_t z) {
  // If the vector length is PTRDIFF_MAX then the maximum index of an element in
  // the vector is PTRDIFF_MAX - 1.
  return (size_t) ((const char *) elmt - (const char *) vector) / z;
}

extern inline
void mx_vector_get(mx_vector_c vector, size_t i, void *elmt, size_t z);

extern inline
void mx_vector_set(mx_vector_t vector, size_t i, const void *elmt, size_t z);

void mx_vector_swap_z(mx_vector_t vector, size_t i, size_t j, size_t z) {
  char *a = mx_vector_at(vector, i, z);
  char *b = mx_vector_at(vector, j, z);

  for (size_t k = 0; k < z; k++) {
    char buffer;
    buffer = a[k];
    a[k] = b[k];
    b[k] = buffer;
  }
}

void mx_vector_move_z(mx_vector_t vector, size_t target, size_t source, size_t z) {
  if (target == source)
    return;

  if (target < source) {
    while (source-- > target)
      mx_vector_swap_z(vector, source, source + 1, z);
  } else {
    for (; source < target; source++)
      mx_vector_swap_z(vector, source, source + 1, z);
  }
}

mx_vector_t mx_vector_resize_z(mx_vector_t vector, size_t volume, size_t z) {
  header_t *header = vector_to_header(vector);
  size_t size;

  // calculate size and test for overflow
  if (__builtin_mul_overflow(volume, z, &size))
    return errno = ENOMEM, NULL;
  if (__builtin_add_overflow(size, sizeof(header_t), &size))
    return errno = ENOMEM, NULL;

  if ((header = realloc(header, size)) == NULL)
    return NULL;

  if ((header->volume = volume) < header->length)
    header->length = volume;

  return header_to_vector(header);
}

mx_vector_t mx_vector_shrink_z(mx_vector_t vector, size_t z) {
  mx_vector_t shrunk;

  shrunk = mx_vector_resize_z(vector, mx_vector_length(vector), z);

  return shrunk != NULL ? shrunk : vector;
}

mx_vector_t mx_vector_ensure_z(mx_vector_t vector, size_t length, size_t z) {
  if (length <= mx_vector_volume(vector))
    return vector;

  // just volume = (length * 8 + 3) / 5 avoiding intermediate overflow
  size_t volume = length / 5 * 8 + ((length % 5) * 8 + 3) / 5;

  // if the volume doesn't overflow then attempt to allocate it
  if (volume > length) {
    mx_vector_t resize;
    if ((resize = mx_vector_resize_z(vector, volume, z)) != NULL)
      return resize;
  }

  // if either the volume overflows or the allocation failed then attempt to
  // resize to just the length
  return mx_vector_resize_z(vector, length, z);
}

mx_vector_t
mx_vector_insert_z(mx_vector_t vector, size_t i, const void *elmt, size_t z) {
  return mx_vector_inject_z(vector, i, elmt, 1, z);
}

mx_vector_t mx_vector_inject_z(
    mx_vector_t vector, size_t i, const void *elmt, size_t n, size_t z) {
  size_t length = mx_vector_length(vector);

  if (__builtin_add_overflow(length, n, &length))
    return errno = ENOMEM, NULL;

  if ((vector = mx_vector_ensure_z(vector, length, z)) == NULL)
    return NULL;

  // move the existing elements n elements toward the tail
  void *target = mx_vector_at(vector, i + n, z);
  void *source = mx_vector_at(vector, i + 0, z);
  size_t size = (mx_vector_length(vector) - i) * z;
  memmove(target, source, size);

  if (elmt != NULL)
    memcpy(mx_vector_at(vector, i, z), elmt, n * z);

  // increase the length
  vector_to_header(vector)->length = length;

  return vector;
}

mx_vector_t mx_vector_remove_z(mx_vector_t vector, size_t i, size_t z) {
  return mx_vector_excise_z(vector, i, 1, z);
}

mx_vector_t mx_vector_excise_z(mx_vector_t vector, size_t i, size_t n, size_t z) {
  size_t length = mx_vector_length(vector) - n;

  // move the existing elements n elements toward the head
  void *target = mx_vector_at(vector, i + 0, z);
  void *source = mx_vector_at(vector, i + n, z);
  size_t size = (length - i) * z;
  memmove(target, source, size);

  if (length <= (mx_vector_volume(vector) - 1) / 2) {
    mx_vector_t resize;
    // just volume = (length * 6 + 4) / 5 avoiding intermediate overflow
    size_t volume = length / 5 * 6 + ((length % 5) * 6 + 4) / 5;

    if ((resize = mx_vector_resize_z(vector, volume, z)) != NULL)
      vector = resize;
  }

  // decrease the length
  vector_to_header(vector)->length = length;

  return vector;
}

mx_vector_t mx_vector_truncate_z(mx_vector_t vector, size_t length, size_t z) {
  size_t n = mx_vector_length(vector) - length;
  return mx_vector_excise_z(vector, mx_vector_length(vector) - n, n, z);
}

mx_vector_t mx_vector_append_z(mx_vector_t vector, const void *elmt, size_t z) {
  return mx_vector_inject_z(vector, mx_vector_length(vector), elmt, 1, z);
}

mx_vector_t
mx_vector_extend_z(mx_vector_t vector, const void *elmt, size_t n, size_t z) {
  return mx_vector_inject_z(vector, mx_vector_length(vector), elmt, n, z);
}

// void *mx_vector_tail_z(mx_vector_t vector, size_t z) {
//   return mx_vector_at(vector, mx_vector_length(vector) - 1, z);
// }

mx_vector_t mx_vector_pull_z(mx_vector_t vector, void *elmt, size_t z) {
  if (elmt != NULL)
    mx_vector_get(vector, mx_vector_length(vector) - 1, elmt, z);
  return mx_vector_remove_z(vector, mx_vector_length(vector) - 1, z);
}

mx_vector_t mx_vector_shift_z(mx_vector_t vector, void *elmt, size_t z) {
  if (elmt != NULL)
    mx_vector_get(vector, 0, elmt, z);
  return mx_vector_remove_z(vector, 0, z);
}

bool mx_vector_eq_z(mx_vector_c a, mx_vector_c b, mx_eq_f eqf, size_t z) {
  if (mx_vector_length(a) != mx_vector_length(b))
    return false;

  for (size_t i = 0; i < mx_vector_length(a); i++) {
    const void *ax = mx_vector_at(a, i, z);
    const void *bx = mx_vector_at(b, i, z);

    // compare using memcpy if no equality function is available
    if (eqf == NULL ? memcmp(ax, bx, z) : !eqf(ax, bx))
      return false;
  }

  return true;
}

bool mx_vector_ne_z(mx_vector_c a, mx_vector_c b, mx_eq_f eqf, size_t z) {
  return !mx_vector_eq_z(a, b, eqf, z);
}

void mx_vector_sort_z(mx_vector_t vector, mx_cmp_f cmpf, size_t z) {
  qsort(vector, mx_vector_length(vector), z, cmpf);
}

size_t mx_vector_find_z(mx_vector_t vector, mx_eq_f eqf, void *data, size_t z) {
  return mx_vector_find_next_z(vector, 0, eqf, data, z);
}

size_t
mx_vector_find_next_z(mx_vector_t vector, size_t i, mx_eq_f eqf, void *data, size_t z) {
  for (; i < mx_vector_length(vector); i++) {
    if (eqf(mx_vector_at(vector, i, z), data))
      return i;
  }
  return MX_ABSENT;
}

size_t
mx_vector_find_last_z(mx_vector_t vector, size_t i, mx_eq_f eqf, void *data, size_t z) {
  for (; i-- > 0;) {
    if (eqf(mx_vector_at(vector, i, z), data))
      return i;
  }
  return MX_ABSENT;
}

size_t mx_vector_search_z(mx_vector_t vector, void *elmt, mx_cmp_f cmpf, size_t z) {
  size_t length = mx_vector_length(vector);
  void *result;

  if ((result = bsearch(elmt, vector, length, z, cmpf)) == NULL)
    return MX_ABSENT;

  size_t i = mx_vector_index(vector, result, z);

  while (i > 0 && cmpf(mx_vector_at(vector, i - 1, z), elmt) == 0)
    i--;

  return i;
}

void mx_vector_debug_z(
    mx_vector_c vector, void (*elmt_debug)(const void *), size_t z) {
  const header_t *header = vector_to_header(vector);
  fprintf(stderr,
    "mx_vector_t(data = %p, utilization = %zu/%zu)",
  header->data, header->length, header->volume);

  if (elmt_debug != NULL) {
    fprintf(stderr, " [ ");
    for (size_t i = 0; i < header->length; i++) {
      if (i > 0)
        fprintf(stderr, ", ");
      elmt_debug(mx_vector_at(vector, i, z));
    }
    fprintf(stderr, " ]");
  }
}

mx_vector_t header_to_vector(header_t *header) {
  return (mx_vector_t) header->data;
}
