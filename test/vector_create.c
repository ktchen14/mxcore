#include <assert.h>
#include <dlfcn.h>
#include <errno.h>
#include <stdlib.h>

#include "../source/vector.h"
#include "test.h"

static int malloc_errno = 0;

void *malloc(size_t size) {
  typeof(malloc) *malloc = dlsym(RTLD_NEXT, "malloc");

  if (malloc_errno != 0)
    return errno = malloc_errno, NULL;
  return malloc(size);
}

static size_t last_z = 0;
mx_vector_t mx_vector_create_as_z(const void *data, size_t length, size_t z) {
  typeof(mx_vector_create_as_z) *mx_vector_create_as_z =
    dlsym(RTLD_NEXT, "mx_vector_create_as_z");
  return mx_vector_create_as_z(data, length, last_z = z);
}

void test_vector_create(void) {
  // When the allocation is unsuccessful it returns NULL with errno retained
  // from malloc()
  malloc_errno = ENOENT;
  errno = 0;
  assert(mx_vector_create() == NULL);
  assert(errno == ENOENT);

  malloc_errno = 0;

  // When the allocation is successful it returns a new vector with length = 0
  // and volume = 0
  int *vector = mx_vector_create();
  assert(mx_vector_length(vector) == 0);
  assert(mx_vector_volume(vector) == 0);

  mx_vector_delete(vector);
}

void test_vector_create_as(void) {
  int data[] = { 0, 1, 2, 3, 4, 5, 6, 7 };
  int *vector;
  size_t length = sizeof(data) / sizeof(data[0]);
  int number = 0;

  // It evaluates its data argument once
  vector = mx_vector_create_as(increment_return(data, number), length);
  assert(number == 1);
  mx_vector_delete(vector);

  // It evaluates its length argument once
  vector = mx_vector_create_as(data, increment_return(length, number));
  assert(number == 2);
  mx_vector_delete(vector);

  // It calls mx_vector_create_as_z() with the element size of data
  mx_vector_delete(mx_vector_create_as(data, length));
  assert(last_z == sizeof(int));

  // With a length that causes the vector's data size, when added to the header
  // size, to overflow a size_t; it returns NULL with errno = ENOMEM.
  length = SIZE_MAX / sizeof(int) - 1;
  errno = 0;
  assert(mx_vector_create_as(data, length) == NULL);
  assert(errno == ENOMEM);

  length = sizeof(data) / sizeof(data[0]);

  // When the allocation is unsuccessful it returns NULL with errno retained
  // from malloc()
  malloc_errno = ENOENT;
  errno = 0;
  vector = mx_vector_create_as(data, length);
  assert(vector == NULL);
  assert(errno == ENOENT);

  malloc_errno = 0;

  // When the allocation is successful it sets both the vector's length and
  // volume to length and copies length elements from data into the vector
  vector = mx_vector_create_as(data, length);
  assert(mx_vector_length(vector) == length);
  for (size_t i = 0; i < mx_vector_length(vector); i++)
    assert(vector[i] == (int) i);
  assert(mx_vector_volume(vector) == length);

  mx_vector_delete(vector);
}

int main() {
  test_vector_create();
  test_vector_create_as();
}
