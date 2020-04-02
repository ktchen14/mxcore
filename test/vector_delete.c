#include <assert.h>
#include <dlfcn.h>
#include <errno.h>
#include <stdlib.h>

#include "../source/vector.h"

static void *free_object = NULL;
void stub_free(void *object) {
  free(free_object = object);
}

int main() {
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
