#include <assert.h>
#include <dlfcn.h>
#include <errno.h>
#include <stdlib.h>

#include "../source/vector.h"

static int malloc_errno = 0;

void *malloc(size_t size) {
  typeof(malloc) *malloc = dlsym(RTLD_NEXT, "malloc");

  if (malloc_errno != 0)
    return errno = malloc_errno, NULL;
  return malloc(size);
}

int main() {
  int *vector;

  // When the allocation is unsuccessful it returns NULL with errno retained
  // from malloc()
  malloc_errno = ENOENT;
  errno = 0;
  assert(mx_vector_create() == NULL);
  assert(errno == ENOENT);

  // When the allocation is successful it returns a new vector with length = 0
  // and volume = 0
  malloc_errno = 0;
  assert((vector = mx_vector_create()) != NULL);
  assert(mx_vector_length(vector) == 0);
  assert(mx_vector_volume(vector) == 0);

  mx_vector_delete(vector);
}
