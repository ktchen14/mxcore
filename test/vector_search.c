#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "../source/vector.h"
#include "test.h"

bool eqintp(const void *a, const void *b) {
  return *(const int *) a == *(const int *) b;
}

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

  // With an index greater than or equal to the vector's length, it returns
  // SIZE_MAX
  result = vector_find_next(vector, vector_length(vector), eqintp, &data);
  assert(result == SIZE_MAX);

  // When the equality function, when called on the element at the index,
  // returns true, it returns the index itself
  data = 3;
  result = vector_find_next(vector, 4, eqintp, &data);
  assert(result == 4);

  // When the equality function, when called on the element at the index,
  // returns false, it returns the index of the first element after the index
  // for which the equality function, when called with that element, returns
  // true
  data = 5;
  result = vector_find_next(vector, 2, eqintp, &data);
  assert(result == 6);

  // When no elements in the vector at or after the index match, it returns
  // SIZE_MAX
  data = 7;
  result = vector_find_next(vector, 0, eqintp, &data);
  assert(result == SIZE_MAX);

  vector_delete(vector);
}

int main() {
  test_vector_find_next();
}
