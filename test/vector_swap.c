#include <assert.h>
#include <dlfcn.h>
#include <stdlib.h>

#include "../source/vector.h"
#include "test.h"

static size_t last_z;
void mx_vector_swap_z(mx_vector_t vector, size_t i, size_t j, size_t z) {
  typeof(mx_vector_swap_z) *mx_vector_swap_z =
    dlsym(RTLD_NEXT, "mx_vector_swap_z");
  mx_vector_swap_z(vector, i, j, last_z = z);
}

int main() {
  int *vector = mx_vector_define(int, 0, 1, 2, 3);
  int number = 0;

  // It evaluates its vector argument once
  mx_vector_swap((number++, vector), 0, 3);
  assert(number == 1);

  // It evaluates its index i argument once
  mx_vector_swap(vector, (number++, 0), 3);
  assert(number == 2);

  // It evaluates its index j argument once
  mx_vector_swap(vector, 0, (number++, 3));
  assert(number == 3);

  // It calls mx_vector_swap_z() with the element size of the vector
  mx_vector_swap(vector, 0, 3);
  assert(last_z == sizeof(int));

  // It swaps the element at index i with the element at index j in the vector.
  // No other elements in the vector are modified.
  mx_vector_swap(vector, 1, 2);
  assert(vector[0] == 0);
  assert(vector[1] == 2);
  assert(vector[2] == 1);
  assert(vector[3] == 3);

  mx_vector_delete(vector);
}
