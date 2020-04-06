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
  vector = vector_create();
  vector_delete(vector);
  assert(free_object != NULL);

  // It doesn't affect errno
  vector = vector_create();
  errno = ENOENT;
  vector_delete(vector);
  assert(errno == ENOENT);
}
