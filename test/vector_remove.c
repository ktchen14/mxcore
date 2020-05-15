#include <assert.h>
#include <errno.h>
#include <stddef.h>
#include <stdlib.h>

#include <vector.h>
#include "test.h"

static vector_t last_vector;
static size_t last_i;
static size_t last_n;
static size_t last_excise_z;
static vector_t last_result;
vector_t vector_excise_z(vector_t vector, size_t i, size_t n, size_t z) {
  last_vector = vector;
  last_i = i;
  last_n = n;
  last_excise_z = z;
  return last_result = REAL(vector_excise_z)(vector, i, n, z);
}

// vector_excise(), vector_excise_z()

void test_vector_excise(void) {
  int *vector = vector_define(int, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89);
  int number = 0;

  // It evaluates each argument once
  vector = vector_excise((number++, vector), 1, 0);
  assert(number == 1);
  vector = vector_excise(vector, (number++, 1), 0);
  assert(number == 2);
  vector = vector_excise(vector, 1, (number++, 0));
  assert(number == 3);

  // It calls vector_excise_z() with the element size of the vector
  vector = vector_excise(vector, 1, 0);
  assert(last_excise_z == sizeof(vector[0]));

  // Its expansion is an expression
  assert((vector = vector_excise(vector, 1, 0)));

  // It removes length elements at the index from the vector
  vector = vector_excise(vector, 2, 3);
  assert_vector_data(vector, 1, 2, 13, 21, 34, 55, 89);

  // With length zero it returns the vector unmodified
  vector = vector_excise(vector, 2, 0);
  assert_vector_data(vector, 1, 2, 13, 21, 34, 55, 89);

  // When the index is zero and the length is equal to the vector's length it
  // empties the vector
  vector = vector_excise(vector, 0, vector_length(vector));
  assert(vector_length(vector) == 0);

  vector_delete(vector);

  // When the resultant length of the vector is less than the overallocation
  // threshold it reduces the volume of the vector
  vector = vector_define(int, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
      11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
      21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
      31, 32, 33, 34, 35, 36, 37, 38, 39, 40);
  vector = vector_excise(vector, 18, 22);
  size_t volume = 18 / 5 * 6 + ((18 % 5) * 6 + 4) / 5;
  assert(vector_length(vector) == 18);
  assert(vector_volume(vector) == volume);

  vector_delete(vector);
}

// vector_remove(), vector_remove_z()

static size_t last_remove_z;
vector_t vector_remove_z(vector_t vector, size_t i, size_t z) {
  return REAL(vector_remove_z)(vector, i, last_remove_z = z);
}

void test_vector_remove(void) {
  int *vector = vector_define(int, 1, 2, 3, 5, 8, 13, 21, 34);
  int number = 0;

  // It evaluates each argument once
  vector = vector_remove((number++, vector), 2);
  assert(number == 1);
  vector = vector_remove(vector, (number++, 2));
  assert(number == 2);

  // It calls vector_remove_z() with the element size of the vector
  vector = vector_remove(vector, 2);
  assert(last_remove_z == sizeof(vector[0]));

  // Its expansion is an expression
  assert((vector = vector_remove(vector, 2)));

  // It delegates to vector_excise_z() with length as 1
  int *result = vector_remove(vector, 2);
  assert(last_vector == vector);
  assert(last_i == 2);
  assert(last_n == 1);
  assert(last_excise_z == sizeof(vector[0]));
  assert(result == last_result);

  vector_delete(result);
}

// vector_truncate(), vector_truncate_z()

static size_t last_truncate_z;
vector_t vector_truncate_z(vector_t vector, size_t length, size_t z) {
  return REAL(vector_truncate_z)(vector, length, last_truncate_z = z);
}

void test_vector_truncate(void) {
  int *vector = vector_define(int, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89);
  int number = 0;

  // It evaluates each argument once
  vector = vector_truncate((number++, vector), vector_length(vector) - 1);
  assert(number == 1);
  vector = vector_truncate(vector, (number++, vector_length(vector) - 1));
  assert(number == 2);

  // It calls vector_truncate_z() with the element size of the vector
  vector = vector_truncate(vector, vector_length(vector) - 1);
  assert(last_truncate_z == sizeof(vector[0]));

  // Its expansion is an expression
  assert((vector = vector_truncate(vector, vector_length(vector) - 1)));

  // It delegates to vector_excise_z() with i and n calculated from length and
  // the length of the vector
  size_t length = vector_length(vector);
  int *result = vector_truncate(vector, 3);
  assert(last_vector == vector);
  assert(last_i == 3);
  assert(last_n == length - 3);
  assert(last_excise_z == sizeof(vector[0]));
  assert(result == last_result);

  vector_delete(result);
}

int main() {
  test_vector_remove();
  test_vector_excise();
  test_vector_truncate();
}
