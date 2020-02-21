#include <assert.h>

#include "../source/vector.h"

int main() {
  int *vector = mx_vector_define(int, 1, 2, 3, 4, 5, 6, 7, 8);
  int number = 0;

  // Its result is equivalent to pointer addition on the vector
  assert(mx_vector_at(vector, 0, sizeof(int)) == vector + 0);
  assert(mx_vector_at(vector, 4, sizeof(int)) == vector + 4);
  assert(mx_vector_at(vector, 8, sizeof(int)) == vector + 8);

  // It evalutes its vector argument once
  mx_vector_at((number++, vector), 1, sizeof(int));
  assert(number == 1);

  // It evalutes its index argument once
  mx_vector_at(vector, (number++, 1), sizeof(int));
  assert(number == 2);

  // It evalutes its element size argument once
  mx_vector_at(vector, 1, (number++, sizeof(int)));
  assert(number == 3);

  mx_vector_delete(vector);

  // With a const int * its result type is const int *
  const int *vector_c = mx_vector_define(int, 1, 2, 3, 4);
  static_assert(_Generic(mx_vector_at(vector_c, 1, sizeof(int)),
      const int *: 1, int *: 0),
    "result type should be const int *");

  // With a const void * its result type is const void *
  const void *vector_void_c = vector_c;
  static_assert(_Generic(mx_vector_at(vector_void_c, 1, sizeof(int)),
      const void *: 1, void *: 0),
    "result type should be const void *");

  mx_vector_delete((int *) vector_c);

  // With an int * its result type is int *
  int *vector_m = mx_vector_define(int, 1, 2, 3, 4);
  static_assert(_Generic(mx_vector_at(vector_m, 1, sizeof(int)),
      const int *: 0, int *: 1),
    "result type should be int *");

  // With a void * its result type is void *
  void *vector_void_m = vector_m;
  static_assert(_Generic(mx_vector_at(vector_void_m, 1, sizeof(int)),
      const void *: 0, void *: 1),
    "result type should be void *");

  mx_vector_delete(vector_m);

#ifdef MAKE_TEST_WILL_FAIL
  // It emits a compiler warning if its vector argument isn't a pointer
  size_t scalar = 1;
  mx_vector_at(scalar, 0, sizeof(size_t));
#endif /* MAKE_TEST_WILL_FAIL */
}
