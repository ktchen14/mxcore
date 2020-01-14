#include <assert.h>
#include <dlfcn.h>
#include <errno.h>

#include "../source/vector.h"
#include "test.h"

static int resize_errno = 0;
mx_vector_t mx_vector_resize_z(mx_vector_t vector, size_t volume, size_t z) {
  typeof(mx_vector_resize_z) *mx_vector_resize_z =
    dlsym(RTLD_NEXT, "mx_vector_resize_z");

  if (resize_errno != 0)
    return errno = resize_errno, NULL;
  return mx_vector_resize_z(vector, volume, z);
}

static size_t last_z;
mx_vector_t mx_vector_shrink_z(mx_vector_t vector, size_t z) {
  typeof(mx_vector_shrink_z) *mx_vector_shrink_z =
    dlsym(RTLD_NEXT, "mx_vector_shrink_z");
  return mx_vector_shrink_z(vector, last_z = z);
}

int main() {
  int *vector = mx_vector_define(int, 0, 1, 2, 3, 4, 5, 6, 7);
  size_t length;
  int number = 0;

  // It evaluates its vector argument once
  vector = mx_vector_shrink(increment_return(vector, number));
  assert(number == 1);

  // It calls mx_vector_shrink_z() with the element size of the vector
  struct { int x; } (*vector_s)[4] = mx_vector_create();
  vector_s = mx_vector_shrink(vector_s);
  assert(last_z == sizeof(vector_s[0]));
  mx_vector_delete(vector_s);

  // When the resize is unsuccessful the vector is unmodified with errno
  // retained from the resize operation
  vector = mx_vector_ensure(vector, 20);
  length = mx_vector_length(vector);
  size_t volume = mx_vector_volume(vector);

  resize_errno = ENOENT;
  int *result = mx_vector_shrink(vector);
  resize_errno = 0;

  assert(result == vector);
  assert(errno == ENOENT);
  assert(mx_vector_length(result) == length);
  for (size_t i = 0; i < mx_vector_length(vector); i++)
    assert(vector[i] == (int) i);
  assert(mx_vector_volume(result) == volume);

  // When the resize is successful it sets the vector's volume equal to its
  // length. The data in the vector is unmodified.
  vector = mx_vector_ensure(vector, 20);
  length = mx_vector_length(vector);

  vector = mx_vector_shrink(vector);

  assert(mx_vector_length(result) == length);
  for (size_t i = 0; i < mx_vector_length(vector); i++)
    assert(vector[i] == (int) i);
  assert(mx_vector_volume(vector) == length);

  mx_vector_delete(vector);
}
