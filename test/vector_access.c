#include <assert.h>

#include "../source/vector.h"

int main(int argc __attribute__((unused)), char *argv[] __attribute__((unused))) {
  int *vector = mx_vector_define(int, 1, 2, 3, 4);
  int elmt;

  // It copies the element at the index in the vector to the elmt
  elmt = 0;
  mx_vector_get(vector, 1, &elmt, sizeof(int));
  assert(elmt == 2);

  // It copies the elmt to the element at the index in the vector
  elmt = 9;
  mx_vector_set(vector, 3, &elmt, sizeof(int));
  assert(vector[3] == 9);
}
