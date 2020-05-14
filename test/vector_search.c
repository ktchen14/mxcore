#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <vector.h>
#include "test.h"

bool eqintp(const void *a, const void *b) {
  return *(const int *) a == *(const int *) b;
}

static vector_c last_vector;
static size_t last_i;
static bool (*last_eqf)(const void *a, const void *b);
static const void *last_data;
static size_t last_find_next_z;
static size_t last_result;

size_t vector_find_next_z(
    vector_c vector,
    size_t i,
    bool (*eqf)(const void *a, const void *b),
    const void *data,
    size_t z) {
  last_vector = vector;
  last_i = i;
  last_eqf = eqf;
  last_data = data;
  last_find_next_z = z;

  return last_result = REAL(vector_find_next_z)(vector, i, eqf, data, z);
}

// vector_find_next(), vector_find_next_z()

void test_vector_find_next(void) {
  int *vector = vector_define(int, 1, 2, 2, 3, 3, 3, 5, 5, 5, 5, 5);
  int data = 0;
  int number = 0;
  size_t result;

  // It evaluates its vector argument once
  result = vector_find_next((number++, vector), 0, eqintp, &data);
  assert(number == 1);

  // It evaluates its index argument once
  result = vector_find_next(vector, (number++, 0), eqintp, &data);
  assert(number == 2);

  // It evaluates its equality function argument once
  result = vector_find_next(vector, 0, (number++, eqintp), &data);
  assert(number == 3);

  // It evaluates its data argument once
  result = vector_find_next(vector, 0, eqintp, (number++, &data));
  assert(number == 4);

  // It calls vector_find_next_z() with the element size of the vector
  result = vector_find_next(vector, 0, eqintp, &data);
  assert(last_find_next_z == sizeof(vector[0]));

  // When the index is the vector's length it returns SIZE_MAX
  result = vector_find_next(vector, vector_length(vector), eqintp, &data);
  assert(result == SIZE_MAX);

  // When the element at the index in the vector satisfies the equality
  // function, it returns the index
  data = 3;
  result = vector_find_next(vector, 4, eqintp, &data);
  assert(result == 4);

  // When the element at the index in the vector doesn't satisfy the equality
  // function, it returns the index of the first element after the index in the
  // vector that satisfies the equality function.
  data = 5;
  result = vector_find_next(vector, 2, eqintp, &data);
  assert(result == 6);

  // When no elements at or after the index in the vector satisfy the equality
  // function, it returns SIZE_MAX.
  data = 7;
  result = vector_find_next(vector, 0, eqintp, &data);
  assert(result == SIZE_MAX);

  vector_delete(vector);
}

// vector_find(), vector_find_z()

static size_t last_find_z;
size_t vector_find_z(
    vector_c vector,
    bool (*eqf)(const void *a, const void *b),
    const void *data,
    size_t z) {
  return REAL(vector_find_z)(vector, eqf, data, last_find_z = z);
}

void test_vector_find(void) {
  int *vector = vector_define(int, 1, 2, 2, 3, 3, 3, 5, 5, 5, 5, 5);
  int data = 0;
  int number = 0;
  size_t result;

  // It evaluates its vector argument once
  result = vector_find((number++, vector), eqintp, &data);
  assert(number == 1);

  // It evaluates its equality function argument once
  result = vector_find(vector, (number++, eqintp), &data);
  assert(number == 2);

  // It evaluates its data argument once
  result = vector_find(vector, eqintp, (number++, &data));
  assert(number == 3);

  // It calls vector_find_z() with the element size of the vector
  result = vector_find(vector, eqintp, &data);
  assert(last_find_z == sizeof(vector[0]));

  // It delegates to vector_find_next_z() with index as 0
  result = vector_find(vector, eqintp, &data);
  assert(last_vector == vector);
  assert(last_i == 0);
  assert(last_eqf == eqintp);
  assert(last_data == &data);
  assert(last_find_next_z == sizeof(vector[0]));
  assert(result == last_result);

  vector_delete(vector);
}

// vector_find_last(), vector_find_last_z()

static size_t last_find_last_z;
size_t vector_find_last_z(
    vector_c vector,
    size_t i,
    bool (*eqf)(const void *a, const void *b),
    const void *data,
    size_t z) {
  return REAL(vector_find_last_z)(vector, i, eqf, data, last_find_last_z = z);
}

void test_vector_find_last(void) {
  int *vector = vector_define(int, 1, 2, 2, 3, 3, 3, 5, 5, 5, 5, 5);
  int data = 0;
  int number = 0;
  size_t result;

  // It evaluates its vector argument once
  result = vector_find_last((number++, vector), 0, eqintp, &data);
  assert(number == 1);

  // It evaluates its index argument once
  result = vector_find_last(vector, (number++, 0), eqintp, &data);
  assert(number == 2);

  // It evaluates its equality function argument once
  result = vector_find_last(vector, 0, (number++, eqintp), &data);
  assert(number == 3);

  // It evaluates its data argument once
  result = vector_find_last(vector, 0, eqintp, (number++, &data));
  assert(number == 4);

  // It calls vector_find_last_z() with the element size of the vector
  result = vector_find_last(vector, 0, eqintp, &data);
  assert(last_find_last_z == sizeof(vector[0]));

  // With index 0 it returns SIZE_MAX
  assert(vector_find_last(vector, 0, eqintp, &data) == SIZE_MAX);

  // When the index is the vector's length, it returns the index of the last
  // element in the vector that satisfies the equality function
  data = 3;
  result = vector_find_last(vector, vector_length(vector), eqintp, &data);
  assert(result == 5);

  // With an index in the vector, it returns the index of the last element
  // before the index in the vector that satisfies the equality function
  data = 3;
  result = vector_find_last(vector, 5, eqintp, &data);
  assert(result == 4);

  // When no elements before the index in the vector satisfy the equality
  // function, it returns SIZE_MAX.
  data = 0;
  result = vector_find_last(vector, vector_length(vector), eqintp, &data);
  assert(result == SIZE_MAX);

  vector_delete(vector);
}

// vector_search(), vector_search_z()

static int cmpintp_parity(const void *a, const void *b) {
  int ra = *(const int *) a;
  int rb = *(const int *) b;

  if (ra % 2 == 0 && rb % 2 == 1)
    return -1;
  if (ra % 2 == 1 && rb % 2 == 0)
    return 1;
  return ra < rb ? -1 : (ra > rb ? 1 : 0);
}

static size_t last_search_z;
size_t vector_search_z(
    vector_c vector,
    const void *elmt,
    int (*cmpf)(const void *a, const void *b),
    size_t z) {
  return REAL(vector_search_z)(vector, elmt, cmpf, last_search_z = z);
}

void test_vector_search(void) {
  int *vector = vector_define(int, 2, 2, 1, 3, 3, 3, 5, 5, 5, 5, 5);
  int elmt = 4;
  int number = 0;
  size_t result;

  // It evaluates its vector argument once
  result = vector_search((number++, vector), &elmt, cmpintp_parity);
  assert(number == 1);

  // It evaluates its element argument once
  result = vector_search(vector, (number++, &elmt), cmpintp_parity);
  assert(number == 2);

  // It evaluates its comparison function argument once
  result = vector_search(vector, &elmt, (number++, cmpintp_parity));
  assert(number == 3);

  // It calls vector_search_z() with the element size of the vector
  result = vector_search(vector, &elmt, cmpintp_parity);
  assert(last_search_z == sizeof(vector[0]));

  // It returns the index of the first element in the vector that compares equal
  // to the element
  elmt = 5;
  result = vector_search(vector, &elmt, cmpintp_parity);
  assert(result == 6);

  // When no elements in the vector compare equal to the element it returns
  // SIZE_MAX
  elmt = 4;
  result = vector_search(vector, &elmt, cmpintp_parity);
  assert(result == SIZE_MAX);

  vector_delete(vector);
}

int main() {
  test_vector_find_next();
  test_vector_find();
  test_vector_find_last();
  test_vector_search();
}
