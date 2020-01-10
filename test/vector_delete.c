#include <assert.h>
#include <dlfcn.h>
#include <errno.h>
#include <stdlib.h>

#include "../source/vector.h"

static void *free_object = NULL;
void free(void *object) {
  typeof(free) *real_free = dlsym(RTLD_NEXT, "free");
  real_free(free_object = object);
}

int main(int argc __attribute__((unused)), char *argv[] __attribute__((unused))) {
  int *vector;

  // It deallocates the vector
  vector = mx_vector_create();
  mx_vector_delete(vector);
  assert(free_object != NULL);

  // It doesn't affect errno
  vector = mx_vector_create();
  errno = ENOENT;
  mx_vector_delete(vector);
  assert(errno == ENOENT);
}