#include <assert.h>
#include <dlfcn.h>
#include <errno.h>
#include <stdlib.h>

#include "../source/vector.h"

static mx_vector_t last_vector;
static size_t last_length;
static size_t last_volume;
static size_t last_z;

static int *resize_errno = NULL;
mx_vector_t mx_vector_resize_z(mx_vector_t vector, size_t volume, size_t z) {
  typeof(mx_vector_resize_z) *mx_vector_resize_z =
    dlsym(RTLD_NEXT, "mx_vector_resize_z");

  last_vector = vector;
  last_volume = volume;
  last_z = z;

  int e;
  if (resize_errno != NULL && (e = *resize_errno++) != 0)
    return errno = e, NULL;
  return mx_vector_resize_z(vector, volume, last_z = z);
}

mx_vector_t mx_vector_ensure_z(mx_vector_t vector, size_t length, size_t z) {
  typeof(mx_vector_ensure_z) *mx_vector_ensure_z =
    dlsym(RTLD_NEXT, "mx_vector_ensure_z");
  last_vector = vector;
  last_length = length;
  last_z = z;
  return mx_vector_ensure_z(vector, length, z);
}

int main() {
  int *vector = mx_vector_define(int, 0, 1, 2, 3, 4, 5, 6, 7);
  int number = 0;
  size_t volume;

  // It evaluates its vector argument once
  vector = mx_vector_ensure((number++, vector), 10);
  assert(number == 1);

  // It evaluates its volume argument once
  vector = mx_vector_ensure(vector, (number++, 12));
  assert(number == 2);

  // It calls mx_vector_ensure_z() with the element size of the vector
  vector = mx_vector_ensure(vector, 4);
  assert(last_z == sizeof(vector[0]));

  // With a length less than or equal to the vector's volume it returns the
  // vector unmodified
  volume = mx_vector_volume(vector);

  resize_errno = (int[]) { ENOMEM };
  int *result = mx_vector_ensure(vector, mx_vector_volume(vector) - 1);
  resize_errno = NULL;

  assert(result == vector);
  assert(mx_vector_volume(vector) == volume);

  // When the volume calculation overflows it returns the result of a resize to
  // the length (with errno retained if relevant)
  resize_errno = (int[]) { ENOENT };
  assert(mx_vector_ensure(vector, SIZE_MAX - 1) == NULL);
  resize_errno = NULL;
  assert(errno == ENOENT);

  assert(last_vector == vector);
  assert(last_volume == SIZE_MAX - 1);
  assert(last_z == sizeof(int));

  // When the volume calculation doesn't overflow, and when a resize to that
  // volume is successful, it returns the resize result.
  volume = 40 / 5 * 8 + ((40 % 5) * 8 + 3) / 5;
  vector = mx_vector_ensure(vector, 40);
  assert(mx_vector_volume(vector) == volume);

  // When the volume calculation doesn't overflow, and when the resize to that
  // volume is unsuccessful, it returns the result of a resize to the length.
  resize_errno = (int[]) { ENOMEM, 0 };
  vector = mx_vector_ensure(vector, 80);
  resize_errno = NULL;
  assert(mx_vector_volume(vector) == 80);

  mx_vector_delete(vector);
}
