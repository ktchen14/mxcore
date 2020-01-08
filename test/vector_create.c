#include <assert.h>
#include <dlfcn.h>
#include <errno.h>
#include <stdlib.h>

#include "../source/vector.h"

static int malloc_return = 0;

void *malloc(size_t size) {
  typeof(malloc) *real_malloc = dlsym(RTLD_NEXT, "malloc");

  if (malloc_return != 0)
    return errno = malloc_return, NULL;
  return real_malloc(size);
}

typedef struct _header_t {
  size_t volume;
  size_t length;
  char data[] __attribute__((aligned));
} header_t;

int main(int argc __attribute__((unused)), char *argv[] __attribute__((unused))) {
  int *vector;

  // When malloc() is unsuccessful it returns NULL with errno retained
  malloc_return = ENOMEM;

  assert(mx_vector_create() == NULL);
  assert(errno == ENOMEM);

  // When malloc() is successful it returns a new vector with length = 0 and
  // volume = 0
  malloc_return = 0;

  vector = mx_vector_create();
  assert(vector != NULL);
  assert(mx_vector_length(vector) == 0);
  assert(mx_vector_volume(vector) == 0);



  // When the length + header size overflows it returns NULL
  size_t length;
  length = SIZE_MAX - sizeof(header_t) + 1;
}
