#include <assert.h>
#include <stdlib.h>

#include <vector.h>
#include "test.h"

static size_t last_z;
void vector_swap_z(vector_t vector, size_t i, size_t j, size_t z) {
  REAL(vector_swap_z)(vector, i, j, last_z = z);
}

int main() {
  int *vector = vector_define(int, 1, 2, 3, 5, 8, 13);
  int number = 0;

  // It evaluates each argument once
  vector_swap((number++, vector), 0, 3);
  assert(number == 1);
  vector_swap(vector, (number++, 0), 3);
  assert(number == 2);
  vector_swap(vector, 0, (number++, 3));
  assert(number == 3);

  // It calls vector_swap_z() with the element size of the vector
  vector_swap(vector, 0, 3);
  assert(last_z == sizeof(int));

  // Its expansion is an expression
  assert((vector_swap(vector, 0, 0), 1));

  // It swaps the element at index i with the element at index j in the vector.
  // No other elements in the vector are modified.
  vector_swap(vector, 1, 4);
  assert(vector[0] == 1);
  assert(vector[1] == 8);
  assert(vector[2] == 3);
  assert(vector[3] == 5);
  assert(vector[4] == 2);
  assert(vector[5] == 13);

  vector_delete(vector);
}
