/// @file vector.c

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vector.h"

// vector/create.h
extern inline __typeof__(vector_create) vector_create;
extern inline __typeof__(vector_import_z) vector_import_z;

vector_t vector_duplicate_z(vector_c source, size_t z) {
  struct __vector_header_t *header;

  size_t volume = vector_volume(source);
  size_t length = vector_length(source);

  if ((header = malloc(sizeof(*header) + volume * z)) == NULL) {
    if (length == volume)
      return NULL;
    if ((header = malloc(sizeof(*header) + length * z)) == NULL)
      return NULL;
    header->volume = length;
  } else
    header->volume = volume;

  header->length = length;

  return memcpy(header->data, source, length * z);
}

void vector_delete(vector_t vector) {
  free(__vector_to_header(vector));
}

// vector/common.h
extern inline __typeof__(vector_volume) vector_volume;
extern inline __typeof__(vector_length) vector_length;

// vector/access.h
extern inline __typeof__(vector_index) vector_index;
extern inline __typeof__(vector_get) vector_get;
extern inline __typeof__(vector_set) vector_set;

// vector/resize.h
#include "vector/resize.c"
#ifndef VECTOR_TEST
extern inline __typeof__(vector_resize_z) vector_resize_z;
extern inline __typeof__(vector_ensure_z) vector_ensure_z;
extern inline __typeof__(vector_shrink_z) vector_shrink_z;
#endif /* VECTOR_TEST */

// vector/move.h
extern inline __typeof__(vector_move_z) vector_move_z;
extern inline __typeof__(vector_swap_z) vector_swap_z;
extern inline __typeof__(vector_sort_z) vector_sort_z;

// vector/insert.h
#include "vector/insert.c"
#ifndef VECTOR_TEST
extern inline __typeof__(vector_insert_z) vector_insert_z;
extern inline __typeof__(vector_inject_z) vector_inject_z;
extern inline __typeof__(vector_append_z) vector_append_z;
extern inline __typeof__(vector_extend_z) vector_extend_z;
#endif /* VECTOR_TEST */

// vector/remove.h
extern inline __typeof__(vector_remove_z) vector_remove_z;
extern inline __typeof__(vector_excise_z) vector_excise_z;
extern inline __typeof__(vector_truncate_z) vector_truncate_z;

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

// vector/search.h
extern inline __typeof__(vector_find_z) vector_find_z;
extern inline __typeof__(vector_find_next_z) vector_find_next_z;
extern inline __typeof__(vector_find_last_z) vector_find_last_z;

size_t vector_search_z(vector_t vector, void *elmt, cmp_f cmpf, size_t z) {
  size_t length = vector_length(vector);
  void *result;

  if ((result = bsearch(elmt, vector, length, z, cmpf)) == NULL)
    return SIZE_MAX;

  size_t i = vector_index(vector, result, z);

  while (i > 0 && cmpf(vector_at(vector, i - 1, z), elmt) == 0)
    i--;

  return i;
}

void vector_debug_z(
    vector_c vector, void (*elmt_debug)(const void *), size_t z) {
  const struct __vector_header_t *header = __vector_to_header(vector);
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
