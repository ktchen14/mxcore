#include <assert.h>

#include <vector.h>
#include "test.h"

void test_vector_at(void) {
  int *vector = vector_define(int, 1, 2, 3, 4, 5, 6, 7, 8);
  int number = 0;

  // It evaluates each argument once
  vector_at((number++, vector), 1, sizeof(int));
  assert(number == 1);
  vector_at(vector, (number++, 1), sizeof(int));
  assert(number == 2);
  vector_at(vector, 1, (number++, sizeof(int)));
  assert(number == 3);

  // Its expansion is an expression
  assert(vector_at(vector, 0, sizeof(int)));

  // Its result is equivalent to pointer addition on the vector
  assert(vector_at(vector, 0, sizeof(int)) == vector + 0);
  assert(vector_at(vector, 4, sizeof(int)) == vector + 4);
  assert(vector_at(vector, 8, sizeof(int)) == vector + 8);

  vector_delete(vector);

  // With a const int * its result type is const int *
  const int *vector_c = vector_define(int, 1, 2, 3, 4);
  static_assert(_Generic(vector_at(vector_c, 1, sizeof(int)),
      const int *: 1, int *: 0),
    "result type should be const int *");

  // With a const void * its result type is const void *
  const void *vector_void_c = vector_c;
  static_assert(_Generic(vector_at(vector_void_c, 1, sizeof(int)),
      const void *: 1, void *: 0),
    "result type should be const void *");

  vector_delete((int *) vector_c);

  // With an int * its result type is int *
  int *vector_m = vector_define(int, 1, 2, 3, 4);
  static_assert(_Generic(vector_at(vector_m, 1, sizeof(int)),
      const int *: 0, int *: 1),
    "result type should be int *");

  // With a void * its result type is void *
  void *vector_void_m = vector_m;
  static_assert(_Generic(vector_at(vector_void_m, 1, sizeof(int)),
      const void *: 0, void *: 1),
    "result type should be void *");

  vector_delete(vector_m);
}

void test_vector_get(void) {
  int *vector = vector_define(int, 1, 2, 3, 5, 8, 13);

  // It copies the element at the index in the vector to elmt
  int elmt = 0;
  vector_get(vector, 2, &elmt, sizeof(int));
  assert(elmt == 3);

  // When elmt is an element in the vector itself, it copies the element at the
  // index in the vector to elmt
  vector_get(vector, 2, vector + 1, sizeof(int));
  assert_vector_data(vector, 1, 3, 3, 5, 8, 13);

  // When elmt is the element at the index in the vector it doesn't modify the
  // vector
  vector_get(vector, 4, vector + 4, sizeof(int));
  assert_vector_data(vector, 1, 3, 3, 5, 8, 13);

  vector_delete(vector);
}

void test_vector_set(void) {
  int *vector = vector_define(int, 1, 2, 3, 5, 8, 13);

  // It copies elmt to the element at the index in the vector. No other elements
  // in the vector are modified.
  vector_set(vector, 2, &(int) {9}, sizeof(int));
  assert_vector_data(vector, 1, 2, 9, 5, 8, 13);

  // When elmt is an element in the vector itself, it copes the elmt to the
  // element at the index in the vector. No other elements in the vector are
  // modified.
  vector_set(vector, 2, vector + 3, sizeof(int));
  assert_vector_data(vector, 1, 2, 5, 5, 8, 13);

  // When elmt is the element at the index in the vector it doesn't modify the
  // vector
  vector_set(vector, 4, vector + 4, sizeof(int));
  assert_vector_data(vector, 1, 2, 5, 5, 8, 13);

  vector_delete(vector);
}

int main() {
  test_vector_at();
  test_vector_get();
  test_vector_set();
}
