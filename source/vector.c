/// @file source/vector.c

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <vector/common.h>

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
