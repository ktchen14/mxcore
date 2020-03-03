#include <assert.h>
#include <dlfcn.h>
#include <errno.h>
#include <stdlib.h>

#include "../source/vector.h"
#include "test.h"

static size_t last_remove_z;
mx_vector_t mx_vector_remove_z(mx_vector_t vector, size_t i, size_t z) {
  typeof(mx_vector_remove_z) *mx_vector_remove_z =
    dlsym(RTLD_NEXT, "mx_vector_remove_z");
  last_remove_z = z;
  return mx_vector_remove_z(vector, i, z);
}

static mx_vector_t last_vector;
static size_t last_i;
static size_t last_n;
static size_t last_excise_z;
static mx_vector_t last_result;
mx_vector_t mx_vector_excise_z(
    mx_vector_t vector, size_t i, size_t n, size_t z) {
  typeof(mx_vector_excise_z) *mx_vector_excise_z =
    dlsym(RTLD_NEXT, "mx_vector_excise_z");

  last_vector = vector;
  last_i = i;
  last_n = n;
  last_excise_z = z;

  last_result = mx_vector_excise_z(vector, i, n, z);

  return last_result;
}

void test_vector_remove(void) {
  int *vector = mx_vector_define(int, 1, 2, 3, 5, 8, 13, 21, 34);
  int number = 0;

  // It evaluates its vector argument once
  vector = mx_vector_remove((number++, vector), 2);
  assert(number == 1);

  // It evaluates its index argument once
  vector = mx_vector_remove(vector, (number++, 2));
  assert(number == 2);

  // It calls mx_vector_remove_z() with the element size of the vector
  vector = mx_vector_remove(vector, 2);
  assert(last_remove_z == sizeof(vector[0]));

  // It delegates to mx_vector_excise_z() with length as 1
  int *result = mx_vector_remove(vector, 2);
  assert(last_vector == vector);
  assert(last_i == 2);
  assert(last_n == 1);
  assert(last_excise_z == sizeof(vector[0]));
  assert(result == last_result);

  mx_vector_delete(vector);
}

void test_vector_excise(void) {
  int *vector = mx_vector_define(int, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89);
  int number = 0;

  // It evaluates its vector argument once
  vector = mx_vector_excise((number++, vector), 1, 2);
  assert(number == 1);

  // It evaluates its index argument once
  vector = mx_vector_excise(vector, (number++, 1), 2);
  assert(number == 2);

  // It evaluates its length argument once
  vector = mx_vector_excise(vector, 1, (number++, 2));
  assert(number == 3);

  // It calls mx_vector_excise_z() with the element size of the vector
  vector = mx_vector_excise(vector, 1, 2);
  assert(last_excise_z == sizeof(vector[0]));

  mx_vector_delete(vector);

  vector = mx_vector_define(int, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89);

  // It removes length elements at the index from the vector
  vector = mx_vector_excise(vector, 2, 3);
  assert_vector_data(vector, 1, 2, 13, 21, 34, 55, 89);

  // With length zero it returns the vector unmodified
  vector = mx_vector_excise(vector, 2, 0);
  assert_vector_data(vector, 1, 2, 13, 21, 34, 55, 89);

  // When the index is zero and the length is equal to the vector's length it
  // empties the vector
  vector = mx_vector_excise(vector, 0, mx_vector_length(vector));
  assert(mx_vector_length(vector) == 0);

  mx_vector_delete(vector);

  // When the resultant length of the vector is less than the overallocation
  // threshold it reduces the volume of the vector
  vector = mx_vector_define(int, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
      11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
      21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
      31, 32, 33, 34, 35, 36, 37, 38, 39, 40);
  vector = mx_vector_excise(vector, 18, 22);
  size_t volume = 18 / 5 * 6 + ((18 % 5) * 6 + 4) / 5;
  assert(mx_vector_length(vector) == 18);
  assert(mx_vector_volume(vector) == volume);

  mx_vector_delete(vector);
}

int main() {
  test_vector_remove();
  test_vector_excise();
}