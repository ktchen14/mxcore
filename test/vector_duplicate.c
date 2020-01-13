#include <assert.h>
#include <dlfcn.h>
#include <errno.h>
#include <stdlib.h>

#include "../source/vector.h"

// Increment number by 1 and return x
#define increment_return(x, number) ({ number++; x; })

static int *malloc_errno = NULL;

void *malloc(size_t size) {
  typeof(malloc) *malloc = dlsym(RTLD_NEXT, "malloc");

  int e;
  if (malloc_errno == NULL || (e = *malloc_errno++) == 0)
    return malloc(size);
  return errno = e, NULL;
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

  // It evaluates its vector argument once
  int number = 0;
  result = mx_vector_duplicate(increment_return(source, number));
  assert(number == 1);
  mx_vector_delete(result);

  // It calls mx_vector_duplicate_z() with the element size of the vector
  mx_vector_delete(mx_vector_duplicate(source));
  assert(last_z == sizeof(int));

  // When malloc() with the source's volume is unsuccessful, and the source's
  // length is the same as its volume, it returns NULL with errno = ENOMEM
  malloc_errno = (int[]) { ENOMEM };
  assert(mx_vector_duplicate(source_shrunk) == NULL);
  assert(errno == ENOMEM);

  // When malloc() with the source's volume is unsuccessful, and the source's
  // length is different from its volume, and malloc() with the source's length
  // is unsuccessful, it returns NULL
  malloc_errno = (int[]) { ENOMEM, ENOMEM };
  assert(mx_vector_duplicate(source) == NULL);
  assert(errno == ENOMEM);

  // When malloc() with the source's volume is unsuccessful, and the source's
  // length is different from its volume, and malloc() with the source's length
  // is successful, it returns a duplicate vector with the same length as the
  // source and volume = length
  malloc_errno = (int[]) { ENOMEM, 0 };
  result = mx_vector_duplicate(source);

  assert(result != NULL);
  assert(mx_vector_length(result) == mx_vector_length(source));
  assert(mx_vector_volume(result) == mx_vector_length(source));
  for (size_t i = 0; i < mx_vector_length(source); i++)
    assert(result[i] == source[i]);

  mx_vector_delete(result);

  // When malloc() with the source's volume is successful it returns a duplicate
  // vector with the same length and volume as the source
  malloc_errno = (int[]) { 0 };
  result = mx_vector_duplicate(source);

  assert(result != NULL);
  assert(mx_vector_length(result) == mx_vector_length(source));
  assert(mx_vector_volume(result) == mx_vector_volume(source));
  for (size_t i = 0; i < mx_vector_length(source); i++)
    assert(result[i] == source[i]);

  mx_vector_delete(result);
}
