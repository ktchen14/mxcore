#include <assert.h>
#include <dlfcn.h>
#include <errno.h>
#include <stdlib.h>

#include "../source/vector.h"

// Increment number by 1 and return x
#define increment_return(x, number) ({ number++; x; })

static int realloc_return = 0;

void *realloc(void *ptr, size_t size) {
  typeof(realloc) *real_realloc = dlsym(RTLD_NEXT, "realloc");

  if (realloc_return != 0)
    return errno = realloc_return, NULL;
  return real_realloc(ptr, size);
}

static size_t last_z;
mx_vector_t mx_vector_resize_z(mx_vector_t vector, size_t volume, size_t z) {
  typeof(mx_vector_resize_z) *real_mx_vector_resize_z = dlsym(RTLD_NEXT, "mx_vector_resize_z");
  return real_mx_vector_resize_z(vector, volume, last_z = z);
}

int main() {
  int *vector = mx_vector_define(int, 1, 2, 3, 4, 5, 6, 7, 8);
  int number = 0;
  size_t length;
  size_t volume;

  // It evaluates its vector argument once
  vector = mx_vector_resize(increment_return(vector, number), 10);
  assert(number == 1);

  // It evaluates its volume argument once
  vector = mx_vector_resize(vector, increment_return(12, number));
  assert(number == 2);

  // It calls mx_vector_resize() with the element size of the vector
  struct {
    long a; int b; short c; void *e; char n[256];
  } *struct_vector = mx_vector_create();
  struct_vector = mx_vector_resize(struct_vector, 10);
  assert(last_z == sizeof(struct_vector[0]));

  int (*vec)[2] = mx_vector_define(int[2], { 0, 1 }, { 2, 3 });
  vec = mx_vector_resize(vec, 4);
  assert(last_z == sizeof(vec[0]));

  // When volume overflows it returns NULL with errno = ENOMEM
  volume = SIZE_MAX / sizeof(int) + 1;
  errno = 0;
  assert(mx_vector_resize(vector, volume) == NULL);
  assert(errno == ENOMEM);

  volume = SIZE_MAX / sizeof(int);
  errno = 0;
  assert(mx_vector_resize(vector, volume) == NULL);
  assert(errno == ENOMEM);

  // When the allocation is unsuccessful it returns NULL with errno retained
  // from realloc()
  realloc_return = ENOENT;
  errno = 0;
  assert(mx_vector_resize(vector, 40) == NULL);
  assert(errno == ENOENT);

  // With a volume greater than or equal to the length the length is unchanged
  realloc_return = 0;
  volume = 80;
  length = mx_vector_length(vector);
  vector = mx_vector_resize(vector, volume);
  assert(mx_vector_length(vector) == length);

  // With a volume less than the length the length is truncated
  realloc_return = 0;
  volume = 6;
  vector = mx_vector_resize(vector, volume);
  assert(mx_vector_length(vector) == volume);

#ifdef MAKE_TEST_WILL_FAIL
  // It emits a compiler warning if its result is unused
  mx_vector_resize(vector, 20);
#endif /* MAKE_TEST_WILL_FAIL */
}