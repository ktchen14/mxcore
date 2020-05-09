#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "../source/vector.h"
#include "test.h"

bool eqintp(const void *a, const void *b) {
  return *(const int *) a == *(const int *) b;
}

// vector_find_next(), vector_find_next_z()

static size_t last_find_next_z;
size_t vector_find_next_z(
    vector_c vector,
    size_t i,
    bool (*eqf)(const void *a, const void *b),
    const void *data,
    size_t z) {
  return REAL(vector_find_next_z)(vector, i, eqf, data, last_find_next_z = z);
}

void test_vector_find_next(void) {
  int *vector = vector_define(int, 1, 2, 2, 3, 3, 3, 5, 5, 5, 5, 5);
  int data = 0;
  int number = 0;
  size_t result;

  // It evalutes its vector argument once
  result = vector_find_next((number++, vector), 0, eqintp, &data);
  assert(number == 1);

  // It evalutes its index argument once
  result = vector_find_next(vector, (number++, 0), eqintp, &data);
  assert(number == 2);

  // It evalutes its equality function argument once
  result = vector_find_next(vector, 0, (number++, eqintp), &data);
  assert(number == 3);

  // It evaluates its data argument once
  result = vector_find_next(vector, 0, eqintp, (number++, &data));
  assert(number == 4);

  // It calls vector_find_next_z() with the element size of the vector
  result = vector_find_next(vector, 0, eqintp, &data);
  assert(last_find_next_z == sizeof(vector[0]));

  // When the element at the index in the vector satisfies the equality
  // function, it returns the index
  data = 3;
  result = vector_find_next(vector, 4, eqintp, &data);
  assert(result == 4);

  // When the element at the index in the vector doesn't satisfy the equality
  // function, it returns the index of the first element after the index in the
  // vector that satisfies the equality function.
  data = 5;
  result = vector_find_next(vector, 2, eqintp, &data);
  assert(result == 6);

  // When no elements at or after the index in the vector satisfy the equality
  // function, it returns SIZE_MAX.
  data = 7;
  result = vector_find_next(vector, 0, eqintp, &data);
  assert(result == SIZE_MAX);

  vector_delete(vector);
}

// vector_find_last(), vector_find_last_z()

static size_t last_find_last_z;
size_t vector_find_last_z(
    vector_c vector,
    size_t i,
    bool (*eqf)(const void *a, const void *b),
    const void *data,
    size_t z) {
  return REAL(vector_find_last_z)(vector, i, eqf, data, last_find_last_z = z);
}

void test_vector_find_last(void) {
  int *vector = vector_define(int, 1, 2, 2, 3, 3, 3, 5, 5, 5, 5, 5);
  int data = 0;
  int number = 0;
  size_t result;

  // It evalutes its vector argument once
  result = vector_find_last((number++, vector), 0, eqintp, &data);
  assert(number == 1);

  // It evalutes its index argument once
  result = vector_find_last(vector, (number++, 0), eqintp, &data);
  assert(number == 2);

  // It evalutes its equality function argument once
  result = vector_find_last(vector, 0, (number++, eqintp), &data);
  assert(number == 3);

  // It evaluates its data argument once
  result = vector_find_last(vector, 0, eqintp, (number++, &data));
  assert(number == 4);

  // It calls vector_find_last_z() with the element size of the vector
  result = vector_find_last(vector, 0, eqintp, &data);
  assert(last_find_last_z == sizeof(vector[0]));

  // With index 0 it returns SIZE_MAX
  assert(vector_find_last(vector, 0, eqintp, &data) == SIZE_MAX);

  // When the index is the vector's length, it returns the index of the last
  // element in the vector that satisfies the equality function
  data = 3;
  result = vector_find_last(vector, vector_length(vector), eqintp, &data);
  assert(result == 5);

  // With an index in the vector, it returns the index of the last element
  // before the index in the vector that satisfies the equality function
  data = 3;
  result = vector_find_last(vector, 5, eqintp, &data);
  assert(result == 4);

  // When no elements before the index in the vector satisfy the equality
  // function, it returns SIZE_MAX.
  data = 0;
  result = vector_find_last(vector, vector_length(vector), eqintp, &data);
  assert(result == SIZE_MAX);

  vector_delete(vector);
}

int main() {
  test_vector_find_next();
  test_vector_find_last();
}
