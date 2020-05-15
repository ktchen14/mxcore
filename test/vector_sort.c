#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

#include <vector.h>
#include "test.h"

static size_t last_sort_z;
void vector_sort_z(
    vector_t vector,
    int (*cmp)(const void *a, const void *b),
    size_t z) {
  REAL(vector_sort_z)(vector, cmp, last_sort_z = z);
}

static int cmpintp(const void *a, const void *b) {
  int ra = *(const int *) a;
  int rb = *(const int *) b;
  return ra < rb ? -1 : (ra > rb ? 1 : 0);
}

static int cmpintp_parity(const void *a, const void *b) {
  int ra = *(const int *) a;
  int rb = *(const int *) b;

  if (ra % 2 == 0 && rb % 2 == 1)
    return -1;
  if (ra % 2 == 1 && rb % 2 == 0)
    return 1;
  return cmpintp(a, b);
}

int main() {
  int *vector = vector_define(int, 1, 2, 3, 5, 8, 13);
  int number = 0;

  // It evaluates each argument once
  vector_sort((number++, vector), cmpintp);
  assert(number == 1);
  vector_sort(vector, (number++, cmpintp));
  assert(number == 2);

  // It calls vector_sort_z() with the element size of the vector
  vector_sort(vector, cmpintp);
  assert(last_sort_z == sizeof(vector[0]));

  // Its expansion is an expression
  assert((vector_sort(vector, cmpintp), 1));

  // It sorts the vector with the comparator
  vector_sort(vector, cmpintp_parity);
  assert_vector_data(vector, 2, 8, 1, 3, 5, 13);

  vector_delete(vector);
}
