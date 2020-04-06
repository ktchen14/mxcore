#include <assert.h>
#include <dlfcn.h>
#include <stdlib.h>

#include "../source/vector.h"

static size_t last_z;
void vector_swap_z(vector_t vector, size_t i, size_t j, size_t z) {
  typeof(vector_swap_z) *vector_swap_z =
    dlsym(RTLD_NEXT, "vector_swap_z");
  vector_swap_z(vector, i, j, last_z = z);
}

int main() {
  int *vector = vector_define(int, 1, 2, 3, 5, 8, 13);
  int number = 0;

  // It evaluates its vector argument once
  vector_swap((number++, vector), 0, 3);
  assert(number == 1);

  // It evaluates its index i argument once
  vector_swap(vector, (number++, 0), 3);
  assert(number == 2);

  // It evaluates its index j argument once
  vector_swap(vector, 0, (number++, 3));
  assert(number == 3);

  // It calls vector_swap_z() with the element size of the vector
  vector_swap(vector, 0, 3);
  assert(last_z == sizeof(int));

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
