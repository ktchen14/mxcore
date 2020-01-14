#include <assert.h>
#include <dlfcn.h>
#include <stdlib.h>

#include "../source/vector.h"
#include "test.h"

static size_t last_z;
void mx_vector_move_z(mx_vector_t vector, size_t target, size_t source, size_t z) {
  typeof(mx_vector_move_z) *mx_vector_move_z =
    dlsym(RTLD_NEXT, "mx_vector_move_z");
  mx_vector_move_z(vector, target, source, last_z = z);
}

int main() {
  int *vector = mx_vector_define(int, 0, 1, 2, 3);
  int number = 0;

  // It evaluates its vector argument once
  mx_vector_move(increment_return(vector, number), 0, 3);
  assert(number == 1);

  // It evaluates its index i argument once
  mx_vector_move(vector, increment_return(0, number), 3);
  assert(number == 2);

  // It evaluates its index j argument once
  mx_vector_move(vector, 0, increment_return(3, number));
  assert(number == 3);

  // It calls mx_vector_move_z() with the element size of the vector
  mx_vector_move(vector, 0, 3);
  assert(last_z == sizeof(int));

  // When the source index is before the target index, it moves the element at
  // the source index to the target index in the vector
  mx_vector_move(vector, 1, 3);
  assert(vector[0] == 0);
  assert(vector[1] == 3);
  assert(vector[2] == 1);
  assert(vector[3] == 2);

  // When the source index is after the target index, it moves the element at
  // the source index to the target index in the vector
  mx_vector_move(vector, 2, 0);
  assert(vector[0] == 3);
  assert(vector[1] == 1);
  assert(vector[2] == 0);
  assert(vector[3] == 2);

  mx_vector_delete(vector);
}
