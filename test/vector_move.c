#include <assert.h>
#include <stdlib.h>

#include "../source/vector.h"
#include "test.h"

static size_t last_z;
void vector_move_z(vector_t vector, size_t target, size_t source, size_t z) {
  REAL(vector_move_z)(vector, target, source, last_z = z);
}

void test_vector_move(void) {
  int *vector = vector_define(int, 1, 2, 3, 5, 8, 13);
  int number = 0;

  // It evaluates its vector argument once
  vector_move((number++, vector), 0, 3);
  assert(number == 1);

  // It evaluates its index i argument once
  vector_move(vector, (number++, 0), 3);
  assert(number == 2);

  // It evaluates its index j argument once
  vector_move(vector, 0, (number++, 3));
  assert(number == 3);

  // It calls vector_move_z() with the element size of the vector
  vector_move(vector, 0, 3);
  assert(last_z == sizeof(vector[0]));

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

int main() {
  test_vector_move();
}
