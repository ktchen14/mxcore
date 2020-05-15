#include <assert.h>
#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include <vector.h>
#include "test.h"

static int malloc_errno = 0;
__attribute__((used)) void *stub_malloc(size_t size) {
  if (malloc_errno != 0)
    return errno = malloc_errno, NULL;
  return malloc(size);
}

static size_t last_z = 0;
vector_t vector_import_z(const void *data, size_t length, size_t z) {
  return REAL(vector_import_z)(data, length, last_z = z);
}

void test_vector_create(void) {
  // When the allocation is unsuccessful it returns NULL with errno retained
  // from malloc()
  malloc_errno = ENOENT;
  errno = 0;
  assert(vector_create() == NULL);
  assert(errno == ENOENT);

  malloc_errno = 0;

  // When the allocation is successful it returns a new vector with length = 0
  // and volume = 0
  int *vector = vector_create();
  assert(vector_length(vector) == 0);
  assert(vector_volume(vector) == 0);

  vector_delete(vector);
}

void test_vector_import(void) {
  int data[] = { 1, 2, 3, 5, 8, 13, 21, 34 };
  int *vector;
  size_t length = sizeof(data) / sizeof(data[0]);
  int number = 0;

  // It evaluates each argument once
  vector = vector_import((number++, data), length);
  assert(number == 1);
  vector_delete(vector);
  vector = vector_import(data, (number++, length));
  assert(number == 2);
  vector_delete(vector);

  // It calls vector_import_z() with the element size of data
  vector_delete(vector_import(data, length));
  assert(last_z == sizeof(int));

  // Its expansion is an expression
  assert(vector_delete(vector_import(data, length)) == NULL);

  // With a length that causes the vector size, when added to the header size,
  // to overflow a size_t; it returns NULL with errno = ENOMEM.
  errno = 0;
  assert(vector_import(data, SIZE_MAX / sizeof(int) - 1) == NULL);
  assert(errno == ENOMEM);

  // When the allocation is unsuccessful it returns NULL with errno retained
  // from malloc()
  malloc_errno = ENOENT;
  errno = 0;
  vector = vector_import(data, length);
  assert(vector == NULL);
  assert(errno == ENOENT);

  malloc_errno = 0;

  // When the allocation is successful it sets both the vector's length and
  // volume to length and copies length elements from data into the vector
  vector = vector_import(data, length);
  assert(vector_length(vector) == length);
  for (size_t i = 0; i < vector_length(vector); i++)
    assert(vector[i] == data[i]);
  assert(vector_volume(vector) == length);

  vector_delete(vector);
}

int main() {
  test_vector_create();
  test_vector_import();
}
