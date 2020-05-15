#include <assert.h>
#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include <vector.h>
#include "test.h"

static size_t last_z;

static int *resize_errno = NULL;
vector_t vector_resize_z(vector_t vector, size_t volume, size_t z) {
  int e;
  if (resize_errno != NULL && (e = *resize_errno++) != 0)
    return errno = e, NULL;
  return REAL(vector_resize_z)(vector, volume, last_z = z);
}

vector_t vector_ensure_z(vector_t vector, size_t length, size_t z) {
  return REAL(vector_ensure_z)(vector, length, z);
}

int main() {
  int *vector = vector_define(int, 1, 2, 3, 5, 8, 13, 21, 34);
  int number = 0;
  size_t volume;

  // It evaluates each argument once
  vector = vector_ensure((number++, vector), 10);
  assert(number == 1);
  vector = vector_ensure(vector, (number++, 12));
  assert(number == 2);

  // It calls vector_ensure_z() with the element size of the vector
  vector = vector_ensure(vector, 4);
  assert(last_z == sizeof(vector[0]));

  // Its expansion is an expression
  assert((vector = vector_ensure(vector, 4)));

  // With a length less than or equal to the vector's volume it returns the
  // vector unmodified
  volume = vector_volume(vector);

  resize_errno = (int[]) { ENOMEM };
  int *result = vector_ensure(vector, vector_volume(vector) - 1);
  resize_errno = NULL;

  assert(result == vector);
  assert(vector_volume(vector) == volume);

  // When the volume calculation overflows it returns the result of a resize to
  // the length, with errno retained if relevant.
  resize_errno = (int[]) { ENOENT };
  assert(vector_ensure(vector, SIZE_MAX - 1) == NULL);
  resize_errno = NULL;
  assert(errno == ENOENT);

  // When the volume calculation doesn't overflow, and when a resize to that
  // volume is successful, it returns the resize result.
  volume = 40 / 5 * 8 + ((40 % 5) * 8 + 3) / 5;
  vector = vector_ensure(vector, 40);
  assert(vector_volume(vector) == volume);

  // When the volume calculation doesn't overflow, and when the resize to that
  // volume is unsuccessful, it returns the result of a resize to the length.
  resize_errno = (int[]) { ENOMEM, 0 };
  vector = vector_ensure(vector, 80);
  resize_errno = NULL;
  assert(vector_volume(vector) == 80);

  vector_delete(vector);
}
