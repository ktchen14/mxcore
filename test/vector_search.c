#include <assert.h>
#include <stddef.h>
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
  int *vector = vector_define(int, 1, 2, 3, 5, 8, 13);
  int i = 0;
  int number = 0;
  size_t result;

  // It evalutes its vector argument once
  result = vector_find_next((number++, vector), 0, eqintp, &i);
  assert(number == 1);

  // It evalutes its index argument once
  result = vector_find_next(vector, (number++, 0), eqintp, &i);
  assert(number == 2);

  // It evalutes its equality function argument once
  result = vector_find_next(vector, 0, (number++, eqintp), &i);
  assert(number == 3);

  // It evaluates its data argument once
  result = vector_find_next(vector, 0, eqintp, (number++, &i));
  assert(number == 4);

  // It calls vector_find_next_z() with the element size of the vector
  result = vector_find_next(vector, 0, eqintp, &i);
  assert(last_find_next_z == sizeof(vector[0]));

  // It returns the index of the next element
  i = 3;
  result = vector_find_next(vector, 0, eqintp, &(i);
  assert(result == 2);

  vector_delete(vector);
}

int main() {
  test_vector_find_next();
}
