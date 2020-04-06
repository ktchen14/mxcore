#include <assert.h>
#include <dlfcn.h>
#include <errno.h>
#include <stdlib.h>

#include "../source/vector.h"

static int *malloc_errno = NULL;
void *stub_malloc(size_t size) {
  int e;
  if (malloc_errno == NULL || (e = *malloc_errno++) == 0)
    return malloc(size);
  return errno = e, NULL;
}

static size_t last_z;
vector_t vector_duplicate_z(vector_c source, size_t z) {
  typeof(vector_duplicate_z) *vector_duplicate_z =
    dlsym(RTLD_NEXT, "vector_duplicate_z");
  return vector_duplicate_z(source, last_z = z);
}

int main() {
  int *source = vector_define(int, 1, 2, 3, 5, 8, 13, 21, 34);
  int *result;

  // It evaluates its vector argument once
  int number = 0;
  result = vector_duplicate((number++, source));
  assert(number == 1);
  vector_delete(result);

  // It calls vector_duplicate_z() with the element size of the vector
  vector_delete(vector_duplicate(source));
  assert(last_z == sizeof(int));

  // When malloc() with the source's volume is unsuccessful, and the source's
  // length is the same as its volume, it returns NULL with errno = ENOMEM
  source = vector_shrink(source);
  malloc_errno = (int[]) { ENOMEM };
  assert(vector_duplicate(source) == NULL);
  assert(errno == ENOMEM);

  source = vector_ensure(source, 20);

  // When malloc() with the source's volume is unsuccessful, and the source's
  // length is different from its volume, and malloc() with the source's length
  // is unsuccessful, it returns NULL
  malloc_errno = (int[]) { ENOMEM, ENOMEM };
  assert(vector_duplicate(source) == NULL);
  assert(errno == ENOMEM);

  // When malloc() with the source's volume is unsuccessful, and the source's
  // length is different from its volume, and malloc() with the source's length
  // is successful, it returns a duplicate vector with the same length as the
  // source and volume = length
  malloc_errno = (int[]) { ENOMEM, 0 };
  result = vector_duplicate(source);

  assert(result != NULL);
  assert(vector_length(result) == vector_length(source));
  assert(vector_volume(result) == vector_length(source));
  for (size_t i = 0; i < vector_length(source); i++)
    assert(result[i] == source[i]);

  vector_delete(result);

  // When malloc() with the source's volume is successful it returns a duplicate
  // vector with the same length and volume as the source
  malloc_errno = (int[]) { 0 };
  result = vector_duplicate(source);

  assert(result != NULL);
  assert(vector_length(result) == vector_length(source));
  assert(vector_volume(result) == vector_volume(source));
  for (size_t i = 0; i < vector_length(source); i++)
    assert(result[i] == source[i]);

  vector_delete(result);

  vector_delete(source);
}
