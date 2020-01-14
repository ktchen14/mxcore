#include <assert.h>

#include "../source/vector.h"

void test_vector_get(void) {
  int *vector = mx_vector_define(int, 0, 1, 2, 3);

  // It copies the element at the index in the vector to elmt
  int elmt = 0;
  mx_vector_get(vector, 2, &elmt, sizeof(int));
  assert(elmt == 2);

  // When elmt is itself an element in the vector, it copies the element at the
  // index in the vector to elmt
  mx_vector_get(vector, 3, vector + 1, sizeof(int));
  assert(vector[1] == 3);

  mx_vector_delete(vector);
}

void test_vector_set(void) {
  int *vector = mx_vector_define(int, 0, 1, 2, 3);

  // It copies elmt to the element at the index in the vector. No other elements
  // in the vector are modified.
  int elmt = 9;
  mx_vector_set(vector, 2, &elmt, sizeof(int));
  assert(vector[0] == 0);
  assert(vector[1] == 1);
  assert(vector[2] == 9);
  assert(vector[3] == 3);

  // When elmt is itself an element in the vector, it copies elmt to the element
  // at the index in the vector. No other elements in the vector are modified.
  mx_vector_set(vector, 3, vector + 1, sizeof(int));
  assert(vector[0] == 0);
  assert(vector[1] == 1);
  assert(vector[2] == 9);
  assert(vector[3] == 1);

  mx_vector_delete(vector);
}

int main() {
  test_vector_get();
  test_vector_set();
}
