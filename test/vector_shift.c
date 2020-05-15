#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

#include <vector.h>
#include "test.h"

// vector_pull(), vector_pull_z()

static size_t last_pull_z;
vector_t vector_pull_z(vector_t vector, void *elmt, size_t z) {
  return REAL(vector_pull_z)(vector, elmt, last_pull_z = z);
}

void test_vector_pull(void) {
  int *vector = vector_define(int, 1, 2, 3, 5, 8, 13, 21, 34);
  int elmt;
  int number = 0;

  // It evaluates its vector argument once
  vector = vector_pull((number++, vector), NULL);
  assert(number == 1);

  // It evaluates its element argument once
  vector = vector_pull(vector, (number++, NULL));
  assert(number == 2);

  // It calls vector_pull_z() with the element size of the vector
  vector = vector_pull(vector, NULL);
  assert(last_pull_z == sizeof(int));

  // Its expansion is an expression
  assert(vector = vector_pull(vector, NULL));

  vector_delete(vector);
  vector = vector_define(int, 1, 2, 3, 5, 8, 13, 21, 34);

  // When elmt isn't NULL it copies the last element of the vector to elmt
  vector = vector_pull(vector, &elmt);
  assert(elmt == 34);

  // It removes the last element of the vector
  vector = vector_pull(vector, NULL);
  assert_vector_data(vector, 1, 2, 3, 5, 8, 13);

  // When the resultant length of the vector is less than the overallocation
  // threshold it reduces the volume of the vector
  vector = vector_ensure((int *) vector_truncate(vector, 1), 20);
  vector = vector_pull(vector, NULL);
  assert(vector_length(vector) == 0);
  assert(vector_volume(vector) == 0);

  vector_delete(vector);
}

int main() {
  assert(!strcmp(DEFINITION_SOURCE(vector_push), "vector_append"));
  assert(!strcmp(DEFINITION_SOURCE(vector_push_z), "vector_append_z"));
  test_vector_pull();
}
