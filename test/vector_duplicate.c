#include <assert.h>
#include <dlfcn.h>
#include <errno.h>
#include <stdlib.h>

#include "../source/vector.h"

static int *malloc_return = NULL;

void *malloc(size_t size) {
  typeof(malloc) *real_malloc = dlsym(RTLD_NEXT, "malloc");

  if (malloc_return == NULL)
    return real_malloc(size);

  int e;
  if ((e = *malloc_return++) != 0)
    return errno = e, NULL;
  return real_malloc(size);
}

static size_t last_z;
mx_vector_t mx_vector_duplicate_z(mx_vector_c source, size_t z) {
  typeof(mx_vector_duplicate_z) *mx_vector_duplicate_z = dlsym(RTLD_NEXT, "mx_vector_duplicate_z");
  return mx_vector_duplicate_z(source, last_z = z);
}

int main() {
  int *source = mx_vector_define(int, 1, 2, 3, 4);
  source = mx_vector_ensure(source, 20);

  int *source_shrunk = mx_vector_define(int, 1, 2, 3, 4);
  source_shrunk = mx_vector_shrink(source_shrunk);

  int *result;

  // It passes the element size of the vector to mx_vector_duplicate_z()
  mx_vector_delete(mx_vector_duplicate(source));
  assert(last_z == sizeof(int));

  // When malloc() with the source's volume is unsuccessful, and the source's
  // length is the same as its volume, it returns NULL with errno = ENOMEM
  malloc_return = (int[]) { ENOMEM };
  assert(mx_vector_duplicate(source_shrunk) == NULL);
  assert(errno == ENOMEM);

  // When malloc() with the source's volume is unsuccessful, and the source's
  // length is different from its volume, and malloc() with the source's length
  // is unsuccessful, it returns NULL
  malloc_return = (int[]) { ENOMEM, ENOMEM };
  assert(mx_vector_duplicate(source) == NULL);
  assert(errno == ENOMEM);

  // When malloc() with the source's volume is unsuccessful, and the source's
  // length is different from its volume, and malloc() with the source's length
  // is successful, it returns a duplicate vector with the same length as the
  // source and volume = length
  malloc_return = (int[]) { ENOMEM, 0 };
  result = mx_vector_duplicate(source);

  assert(result != NULL);
  assert(mx_vector_length(result) == mx_vector_length(source));
  assert(mx_vector_volume(result) == mx_vector_length(source));
  for (size_t i = 0; i < mx_vector_length(source); i++)
    assert(result[i] == source[i]);

  mx_vector_delete(result);

  // When malloc() with the source's volume is successful it returns a duplicate
  // vector with the same length and volume as the source
  malloc_return = (int[]) { 0 };
  result = mx_vector_duplicate(source);

  assert(result != NULL);
  assert(mx_vector_length(result) == mx_vector_length(source));
  assert(mx_vector_volume(result) == mx_vector_volume(source));
  for (size_t i = 0; i < mx_vector_length(source); i++)
    assert(result[i] == source[i]);

  mx_vector_delete(result);
}
