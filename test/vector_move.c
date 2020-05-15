#include <assert.h>
#include <stdlib.h>

#include <vector.h>
#include "test.h"

/* vector_move(), vector_move_z() */

static size_t last_move_z;
void vector_move_z(vector_t vector, size_t target, size_t source, size_t z) {
  REAL(vector_move_z)(vector, target, source, last_move_z = z);
}

void test_vector_move(void) {
  int *vector = vector_define(int, 1, 2, 3, 5, 8, 13);
  int number = 0;

  // It evaluates each argument once
  vector_move((number++, vector), 0, 0);
  assert(number == 1);
  vector_move(vector, (number++, 0), 0);
  assert(number == 2);
  vector_move(vector, 0, (number++, 0));
  assert(number == 3);

  // It calls vector_move_z() with the element size of the vector
  vector_move(vector, 0, 0);
  assert(last_move_z == sizeof(vector[0]));

  // Its expansion is an expression
  assert((vector_move(vector, 0, 0), 1));

  // When the source index is before the target index, it moves the element at
  // the source index to the target index in the vector.
  vector_move(vector, 2, 0);
  assert_vector_data(vector, 2, 3, 1, 5, 8, 13);

  // When the source index is after the target index, it moves the element at
  // the source index to the target index in the vector.
  vector_move(vector, 1, 4);
  assert_vector_data(vector, 2, 8, 3, 1, 5, 13);

  // When the source and target indices are the same the vector is unmodified
  vector_move(vector, 3, 3);
  assert_vector_data(vector, 2, 8, 3, 1, 5, 13);

  vector_delete(vector);
}

/* vector_swap(), vector_swap_z() */

static size_t last_swap_z;
void vector_swap_z(vector_t vector, size_t target, size_t source, size_t z) {
  REAL(vector_swap_z)(vector, target, source, last_swap_z = z);
}

void test_vector_swap(void) {
  int *vector = vector_define(int, 1, 2, 3, 5, 8, 13);
  int number = 0;

  // It evaluates each argument once
  vector_swap((number++, vector), 0, 0);
  assert(number == 1);
  vector_swap(vector, (number++, 0), 0);
  assert(number == 2);
  vector_swap(vector, 0, (number++, 0));
  assert(number == 3);

  // It calls vector_swap_z() with the element size of the vector
  vector_swap(vector, 0, 0);
  assert(last_swap_z == sizeof(vector[0]));

  // Its expansion is an expression
  assert((vector_swap(vector, 0, 0), 1));

  // It swaps the element at index i with the element at index j
  vector_swap(vector, 1, 5);
  assert_vector_data(vector, 1, 13, 3, 5, 8, 2);

  // When the indices i and j are the same the vector is unmodified
  vector_swap(vector, 2, 2);
  assert_vector_data(vector, 1, 13, 3, 5, 8, 2);

  vector_delete(vector);
}

int main() {
  test_vector_move();
  test_vector_swap();
}
