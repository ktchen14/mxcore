#include <assert.h>
#include <errno.h>
#include <stdlib.h>

#include <vector.h>

static void *free_object = NULL;
__attribute__((used)) void stub_free(void *object) {
  free(free_object = object);
}

int main() {
  int *vector;

  // It deallocates the vector
  vector = vector_create();
  vector_delete(vector);
  assert(free_object != NULL);

  // It returns NULL
  assert(vector_delete(vector_create()) == NULL);

  // It doesn't affect errno
  vector = vector_create();
  errno = ENOENT;
  vector_delete(vector);
  assert(errno == ENOENT);
}
