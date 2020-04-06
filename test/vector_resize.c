#include <assert.h>
#include <dlfcn.h>
#include <errno.h>
#include <stdlib.h>

#include "../source/vector.h"

static int realloc_return = 0;

void *stub_realloc(void *data, size_t size) {
  if (realloc_return != 0)
    return errno = realloc_return, NULL;
  return realloc(data, size);
}

static size_t last_z;
vector_t vector_resize_z(vector_t vector, size_t volume, size_t z) {
  typeof(vector_resize_z) *vector_resize_z =
    dlsym(RTLD_NEXT, "vector_resize_z");
  return vector_resize_z(vector, volume, last_z = z);
}

int main() {
  int *vector = vector_define(int, 1, 2, 3, 4, 5, 6, 7, 8);
  int number = 0;
  size_t length;
  size_t volume;

  // It evaluates its vector argument once
  vector = vector_resize((number++, vector), 10);
  assert(number == 1);

  // It evaluates its volume argument once
  vector = vector_resize(vector, (number++, 12));
  assert(number == 2);

  // It calls vector_resize() with the element size of the vector
  vector = vector_resize(vector, 10);
  assert(last_z == sizeof(vector[0]));

  // With a volume that causes the data size to overflow a size_t, it returns
  // NULL with errno = ENOMEM
  volume = SIZE_MAX / sizeof(int) + 1;
  errno = 0;
  assert(vector_resize(vector, volume) == NULL);
  assert(errno == ENOMEM);

  // With a volume that causes the vector size to overflow a size_t, it returns
  // NULL with errno = ENOMEM
  volume = SIZE_MAX / sizeof(int);
  errno = 0;
  assert(vector_resize(vector, volume) == NULL);
  assert(errno == ENOMEM);

  // When the reallocation is unsuccessful it returns NULL with errno retained
  // from realloc()
  realloc_return = ENOENT;
  errno = 0;
  assert(vector_resize(vector, 40) == NULL);
  assert(errno == ENOENT);

  realloc_return = 0;

  // With a volume greater than or equal to the vector's length, the vector's
  // length is unchanged
  length = vector_length(vector);
  vector = vector_resize(vector, length + 2);
  assert(vector_length(vector) == length);

  // With a volume less than the vector's length it truncates the vector to the
  // volume
  vector = vector_resize(vector, length - 2);
  assert(vector_length(vector) == vector_volume(vector));

  vector_delete(vector);

#ifdef MAKE_TEST_WILL_FAIL
  // It emits a compiler warning if its result is unused
  vector_resize(vector, 20);
#endif /* MAKE_TEST_WILL_FAIL */
}
