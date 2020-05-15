#include <assert.h>
#include <errno.h>

#include <vector.h>
#include "test.h"

static int resize_errno = 0;
vector_t vector_resize_z(vector_t vector, size_t volume, size_t z) {
  if (resize_errno != 0)
    return errno = resize_errno, NULL;
  return REAL(vector_resize_z)(vector, volume, z);
}

static size_t last_z;
vector_t vector_shrink_z(vector_t vector, size_t z) {
  return REAL(vector_shrink_z)(vector, last_z = z);
}

#include <stdio.h>

int main() {
  int *vector = vector_define(int, 0, 1, 2, 3, 4, 5, 6, 7);
  size_t length;
  int number = 0;

  // It evaluates each argument once
  vector = vector_shrink((number++, vector));
  assert(number == 1);

  // It calls vector_shrink_z() with the element size of the vector
  vector = vector_shrink(vector);
  assert(last_z == sizeof(vector[0]));

  // Its expansion is an expression
  assert((vector = vector_shrink(vector)));

  // When the resize is unsuccessful the vector is unmodified with errno
  // retained from the resize operation
  vector = vector_ensure(vector, 20);
  length = vector_length(vector);
  size_t volume = vector_volume(vector);

  resize_errno = ENOENT;
  int *result = vector_shrink(vector);
  resize_errno = 0;

  assert(result == vector);
  assert(errno == ENOENT);
  assert(vector_length(result) == length);
  for (size_t i = 0; i < vector_length(vector); i++)
    assert(vector[i] == (int) i);
  assert(vector_volume(result) == volume);

  vector = result;

  // When the resize is successful it sets the vector's volume equal to its
  // length. The data in the vector is unmodified.
  vector = vector_ensure(vector, 20);
  length = vector_length(vector);

  vector = vector_shrink(vector);

  assert(vector_length(vector) == length);
  for (size_t i = 0; i < vector_length(vector); i++)
    assert(vector[i] == (int) i);
  assert(vector_volume(vector) == length);

  vector_delete(vector);
}
