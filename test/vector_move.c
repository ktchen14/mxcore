#include <assert.h>
#include <dlfcn.h>
#include <stdlib.h>

#include "../source/vector.h"

static size_t last_z;
void mx_vector_move_z(mx_vector_t vector, size_t target, size_t source, size_t z) {
  typeof(mx_vector_move_z) *mx_vector_move_z =
    dlsym(RTLD_NEXT, "mx_vector_move_z");
  mx_vector_move_z(vector, target, source, last_z = z);
}

int main() {
  int *vector = mx_vector_define(int, 1, 2, 3, 5, 8, 13);
  int number = 0;

  // It evaluates its vector argument once
  mx_vector_move((number++, vector), 0, 3);
  assert(number == 1);

  // It evaluates its index i argument once
  mx_vector_move(vector, (number++, 0), 3);
  assert(number == 2);

  // It evaluates its index j argument once
  mx_vector_move(vector, 0, (number++, 3));
  assert(number == 3);

  // It calls mx_vector_move_z() with the element size of the vector
  mx_vector_move(vector, 0, 3);
  assert(last_z == sizeof(int));

  // When the source index is before the target index, it moves the element at
  // the source index to the target index in the vector.
  mx_vector_move(vector, 2, 0);
  assert(vector[0] == 2);
  assert(vector[1] == 3);
  assert(vector[2] == 1);
  assert(vector[3] == 5);
  assert(vector[4] == 8);
  assert(vector[5] == 13);

  // When the source index is after the target index, it moves the element at
  // the source index to the target index in the vector.
  mx_vector_move(vector, 1, 4);
  assert(vector[0] == 2);
  assert(vector[1] == 8);
  assert(vector[2] == 3);
  assert(vector[3] == 1);
  assert(vector[4] == 5);
  assert(vector[5] == 13);

  mx_vector_delete(vector);
}
