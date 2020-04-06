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
static inline vector_t header_to_vector(header_t *header)
  __attribute__((const, nonnull, returns_nonnull));

/**
 * @brief Return the header associated with the @a vector
 *
 * This function is @a const qualified on the @a vector. That is if the element
 * type of the @a vector is @c const qualified (@a vector is a @c vector_c)
 * then this will return a <tt>const header_t *</tt>. Otherwise this will return
 * a <tt>header_t *</tt>.
 */
#define vector_to_header(vector) ({ \
  _Pragma("GCC diagnostic push"); \
  _Pragma("GCC diagnostic ignored \"-Wcast-align\""); \
  _Pragma("GCC diagnostic ignored \"-Wcast-qual\""); \
  _Generic((vector), vector_t: (/* */ header_t *) ( \
      (/* */ char *) (vector) - offsetof(header_t, data) \
    ), vector_c: (const header_t *) ( \
      (const char *) (vector) - offsetof(header_t, data) \
    )); \
  _Pragma("GCC diagnostic pop") \
})

vector_t vector_create() {
  header_t *header;

  if ((header = malloc(sizeof(header_t))) == NULL)
    return NULL;

  header->volume = 0;
  header->length = 0;

  return header_to_vector(header);
}

vector_t vector_import_z(const void *data, size_t length, size_t z) {
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

vector_t vector_duplicate_z(vector_c source, size_t z) {
  header_t *header;

  size_t volume = vector_volume(source);
  size_t length = vector_length(source);

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

void vector_delete(vector_t vector) {
  free(vector_to_header(vector));
}

size_t vector_volume(vector_c vector) {
  return vector_to_header(vector)->volume;
}

size_t vector_length(vector_c vector) {
  return vector_to_header(vector)->length;
}

size_t vector_index(vector_c vector, const void *elmt, size_t z) {
  // If the vector length is PTRDIFF_MAX then the maximum index of an element in
  // the vector is PTRDIFF_MAX - 1.
  return (size_t) ((const char *) elmt - (const char *) vector) / z;
}

// Access
extern inline __typeof__(vector_get) vector_get;
extern inline __typeof__(vector_set) vector_set;

void vector_swap_z(vector_t vector, size_t i, size_t j, size_t z) {
  char *a = vector_at(vector, i, z);
  char *b = vector_at(vector, j, z);

  for (size_t k = 0; k < z; k++) {
    char buffer;
    buffer = a[k];
    a[k] = b[k];
    b[k] = buffer;
  }
}

void vector_move_z(vector_t vector, size_t target, size_t source, size_t z) {
  if (target == source)
    return;

  if (target < source) {
    while (source-- > target)
      vector_swap_z(vector, source, source + 1, z);
  } else {
    for (; source < target; source++)
      vector_swap_z(vector, source, source + 1, z);
  }
}

vector_t vector_resize_z(vector_t vector, size_t volume, size_t z) {
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

vector_t vector_shrink_z(vector_t vector, size_t z) {
  vector_t shrunk;

  shrunk = vector_resize_z(vector, vector_length(vector), z);

  return shrunk != NULL ? shrunk : vector;
}

vector_t vector_ensure_z(vector_t vector, size_t length, size_t z) {
  if (length <= vector_volume(vector))
    return vector;

  // just volume = (length * 8 + 3) / 5 avoiding intermediate overflow
  size_t volume = length / 5 * 8 + ((length % 5) * 8 + 3) / 5;

  // if the volume doesn't overflow then attempt to allocate it
  if (volume > length) {
    vector_t resize;
    if ((resize = vector_resize_z(vector, volume, z)) != NULL)
      return resize;
  }

  // if either the volume overflows or the allocation failed then attempt to
  // resize to just the length
  return vector_resize_z(vector, length, z);
}

vector_t
vector_insert_z(vector_t vector, size_t i, const void *elmt, size_t z) {
  return vector_inject_z(vector, i, elmt, 1, z);
}

vector_t vector_inject_z(
    vector_t vector, size_t i, const void *elmt, size_t n, size_t z) {
  size_t length = vector_length(vector);

  if (__builtin_add_overflow(length, n, &length))
    return errno = ENOMEM, NULL;

  if ((vector = vector_ensure_z(vector, length, z)) == NULL)
    return NULL;

  // move the existing elements n elements toward the tail
  void *target = vector_at(vector, i + n, z);
  void *source = vector_at(vector, i + 0, z);
  size_t size = (vector_length(vector) - i) * z;
  memmove(target, source, size);

  if (elmt != NULL)
    memcpy(vector_at(vector, i, z), elmt, n * z);

  // increase the length
  vector_to_header(vector)->length = length;

  return vector;
}

vector_t vector_remove_z(vector_t vector, size_t i, size_t z) {
  return vector_excise_z(vector, i, 1, z);
}

vector_t vector_excise_z(vector_t vector, size_t i, size_t n, size_t z) {
  size_t length = vector_length(vector) - n;

  // move the existing elements n elements toward the head
  void *target = vector_at(vector, i + 0, z);
  void *source = vector_at(vector, i + n, z);
  size_t size = (length - i) * z;
  memmove(target, source, size);

  if (length <= (vector_volume(vector) - 1) / 2) {
    vector_t resize;
    // just volume = (length * 6 + 4) / 5 avoiding intermediate overflow
    size_t volume = length / 5 * 6 + ((length % 5) * 6 + 4) / 5;

    if ((resize = vector_resize_z(vector, volume, z)) != NULL)
      vector = resize;
  }

  // decrease the length
  vector_to_header(vector)->length = length;

  return vector;
}

vector_t vector_truncate_z(vector_t vector, size_t length, size_t z) {
  size_t n = vector_length(vector) - length;
  return vector_excise_z(vector, vector_length(vector) - n, n, z);
}

vector_t vector_append_z(vector_t vector, const void *elmt, size_t z) {
  return vector_inject_z(vector, vector_length(vector), elmt, 1, z);
}

vector_t
vector_extend_z(vector_t vector, const void *elmt, size_t n, size_t z) {
  return vector_inject_z(vector, vector_length(vector), elmt, n, z);
}

// void *vector_tail_z(vector_t vector, size_t z) {
//   return vector_at(vector, vector_length(vector) - 1, z);
// }

vector_t vector_pull_z(vector_t vector, void *elmt, size_t z) {
  if (elmt != NULL)
    vector_get(vector, vector_length(vector) - 1, elmt, z);
  return vector_remove_z(vector, vector_length(vector) - 1, z);
}

vector_t vector_shift_z(vector_t vector, void *elmt, size_t z) {
  if (elmt != NULL)
    vector_get(vector, 0, elmt, z);
  return vector_remove_z(vector, 0, z);
}

bool vector_eq_z(vector_c a, vector_c b, eq_f eqf, size_t z) {
  if (vector_length(a) != vector_length(b))
    return false;

  for (size_t i = 0; i < vector_length(a); i++) {
    const void *ax = vector_at(a, i, z);
    const void *bx = vector_at(b, i, z);

    // compare using memcpy if no equality function is available
    if (eqf == NULL ? memcmp(ax, bx, z) : !eqf(ax, bx))
      return false;
  }

  return true;
}

bool vector_ne_z(vector_c a, vector_c b, eq_f eqf, size_t z) {
  return !vector_eq_z(a, b, eqf, z);
}

void vector_sort_z(vector_t vector, cmp_f cmpf, size_t z) {
  qsort(vector, vector_length(vector), z, cmpf);
}

size_t vector_find_z(vector_t vector, eq_f eqf, void *data, size_t z) {
  return vector_find_next_z(vector, 0, eqf, data, z);
}

size_t
vector_find_next_z(vector_t vector, size_t i, eq_f eqf, void *data, size_t z) {
  for (; i < vector_length(vector); i++) {
    if (eqf(vector_at(vector, i, z), data))
      return i;
  }
  return MX_ABSENT;
}

size_t
vector_find_last_z(vector_t vector, size_t i, eq_f eqf, void *data, size_t z) {
  for (; i-- > 0;) {
    if (eqf(vector_at(vector, i, z), data))
      return i;
  }
  return MX_ABSENT;
}

size_t vector_search_z(vector_t vector, void *elmt, cmp_f cmpf, size_t z) {
  size_t length = vector_length(vector);
  void *result;

  if ((result = bsearch(elmt, vector, length, z, cmpf)) == NULL)
    return MX_ABSENT;

  size_t i = vector_index(vector, result, z);

  while (i > 0 && cmpf(vector_at(vector, i - 1, z), elmt) == 0)
    i--;

  return i;
}

void vector_debug_z(
    vector_c vector, void (*elmt_debug)(const void *), size_t z) {
  const header_t *header = vector_to_header(vector);
  fprintf(stderr,
    "vector_t(data = %p, utilization = %zu/%zu)",
  header->data, header->length, header->volume);

  if (elmt_debug != NULL) {
    fprintf(stderr, " [ ");
    for (size_t i = 0; i < header->length; i++) {
      if (i > 0)
        fprintf(stderr, ", ");
      elmt_debug(vector_at(vector, i, z));
    }
    fprintf(stderr, " ]");
  }
}

vector_t header_to_vector(header_t *header) {
  return (vector_t) header->data;
}
