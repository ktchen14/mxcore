#include <assert.h>
#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include <vector.h>
#include "test.h"

static int ensure_errno = 0;
vector_t vector_ensure_z(vector_t vector, size_t length, size_t z) {
  if (ensure_errno != 0)
    return errno = ensure_errno, NULL;
  return REAL(vector_ensure_z)(vector, length, z);
}

static size_t last_insert_z;
vector_t vector_insert_z(
    vector_t vector, size_t i, const void *elmt, size_t z) {
  return REAL(vector_insert_z)(vector, i, elmt, last_insert_z = z);
}

static vector_t last_vector;
static size_t last_i;
static const void *last_elmt;
static size_t last_n;
static size_t last_inject_z;
static vector_t last_result;

vector_t vector_inject_z(
    vector_t vector, size_t i, const void *elmt, size_t n, size_t z) {
  last_vector = vector;
  last_i = i;
  last_elmt = elmt;
  last_n = n;
  last_inject_z = z;

  return last_result = REAL(vector_inject_z)(vector, i, elmt, n, z);
}

static size_t last_append_z;
vector_t vector_append_z(vector_t vector, const void *elmt, size_t z) {
  return REAL(vector_append_z)(vector, elmt, last_append_z = z);
}

static size_t last_extend_z;
vector_t vector_extend_z(
    vector_t vector, const void *elmt, size_t n, size_t z) {
  return REAL(vector_extend_z)(vector, elmt, n, last_extend_z = z);
}

void test_vector_insert(void) {
  int *vector = vector_define(int, 1, 2, 3, 5, 8, 13);
  int data = 13;
  int number = 0;

  // It evaluates each argument once
  vector = vector_insert((number++, vector), 2, &data);
  assert(number == 1);
  vector = vector_insert(vector, (number++, 2), &data);
  assert(number == 2);
  vector = vector_insert(vector, 2, (number++, &data));
  assert(number == 3);

  // It calls vector_insert_z() with the element size of the vector
  vector = vector_insert(vector, 2, &data);
  assert(last_insert_z == sizeof(vector[0]));

  // Its expansion is an expression
  assert((vector = vector_insert(vector, 2, &data)));

  // It delegates to vector_inject_z() with length as 1
  int *result = vector_insert(vector, 2, &data);
  assert(last_vector == vector);
  assert(last_i == 2);
  assert(last_elmt == &data);
  assert(last_n == 1);
  assert(last_inject_z == sizeof(vector[0]));
  assert(result == last_result);

  vector_delete(result);
}

void test_vector_inject(void) {
  int *vector = vector_define(int, 0, 1, 2, 3, 4, 5, 6, 7);
  int *result;
  int data[] = { 9, 11 };
  size_t data_length = sizeof(data) / sizeof(data[0]);
  int number = 0;

  // It evaluates each argument once
  vector = vector_inject((number++, vector), 2, &data, data_length);
  assert(number == 1);
  vector = vector_inject(vector, (number++, 2), &data, data_length);
  assert(number == 2);
  vector = vector_inject(vector, 2, (number++, &data), data_length);
  assert(number == 3);
  vector = vector_inject(vector, 2, &data, (number++, data_length));
  assert(number == 4);

  // It calls vector_inject_z() with the element size of the vector
  vector = vector_inject(vector, 2, &data, data_length);
  assert(last_inject_z == sizeof(vector[0]));

  // Its expansion is an expression
  assert((vector = vector_inject(vector, 2, &data, data_length)));

  vector_delete(vector);

  // With a length that, when added to the vector's length, overflows a size_t;
  // it returns NULL with errno = ENOMEM. The vector is unmodified.
  vector = vector_define(int, 1, 2, 3, 5);

  size_t maximum_length = SIZE_MAX - vector_length(vector);
  errno = 0;
  result = vector_inject(vector, 2, &data, maximum_length + 1);
  assert(result == NULL);
  assert(errno == ENOMEM);

  assert_vector_data(vector, 1, 2, 3, 5);

  // When the ensure operation is unsuccessful it returns NULL with errno
  // retained from vector_ensure(). The vector is unmodified.
  ensure_errno = ENOENT;
  errno = 0;
  result = vector_inject(vector, 2, &data, data_length);
  ensure_errno = 0;

  assert(result == NULL);
  assert(errno == ENOENT);

  assert_vector_data(vector, 1, 2, 3, 5);

  // When the element isn't NULL it injects length elements into the vector at
  // the index from the element location
  vector = vector_inject(vector, 2, &data, data_length);
  assert_vector_data(vector, 1, 2, 9, 11, 3, 5);

  // When the element is NULL it injects length uninitialized elements into the
  // vector at the index
  vector = vector_inject(vector, 4, NULL, 1);
  assert_vector_data(vector, 1, 2, 9, 11, 3, 3, 5);
  // TODO

  // When the index is equal to the vector's length it appends length elements
  // onto the vector
  vector = vector_inject(vector, vector_length(vector), &data, 2);
  assert_vector_data(vector, 1, 2, 9, 11, 3, 3, 5, 9, 11);

  vector_delete(vector);
}

void test_vector_append(void) {
  int *vector = vector_define(int, 1, 2, 3, 5, 8, 13);
  int data = 13;
  int number = 0;

  // It evaluates each argument once
  vector = vector_append((number++, vector), &data);
  assert(number == 1);
  vector = vector_append(vector, (number++, &data));
  assert(number == 2);

  // It calls vector_append_z() with the element size of the vector
  vector = vector_append(vector, &data);
  assert(last_append_z == sizeof(vector[0]));

  // Its expansion is an expression
  assert((vector = vector_append(vector, &data)));

  // It delegates to vector_inject_z() with the length and element size of the
  // vector
  int *result = vector_append(vector, &data);
  assert(last_vector == vector);
  assert(last_i == vector_length(vector) - 1);
  assert(last_elmt == &data);
  assert(last_n == 1);
  assert(last_inject_z == sizeof(vector[0]));
  assert(result == last_result);

  vector_delete(result);
}

void test_vector_extend(void) {
  int *vector = vector_define(int, 1, 2, 3, 5, 8, 13);
  int data[] = { 9, 11 };
  size_t data_length = sizeof(data) / sizeof(data[0]);
  int number = 0;

  // It evaluates each argument once
  vector = vector_extend((number++, vector), &data, data_length);
  assert(number == 1);
  vector = vector_extend(vector, (number++, &data), data_length);
  assert(number == 2);
  vector = vector_extend(vector, &data, (number++, data_length));
  assert(number == 3);

  // It calls vector_extend_z() with the element size of the vector
  vector = vector_extend(vector, &data, data_length);
  assert(last_extend_z == sizeof(vector[0]));

  // Its expansion is an expression
  assert((vector = vector_extend(vector, &data, data_length)));

  // It delegates to vector_inject_z() with the length and element size of the
  // vector
  int *result = vector_extend(vector, &data, data_length);
  assert(last_vector == vector);
  assert(last_i == vector_length(vector) - data_length);
  assert(last_elmt == &data);
  assert(last_n == data_length);
  assert(last_inject_z == sizeof(vector[0]));
  assert(result == last_result);

  vector_delete(result);
}

int main() {
  test_vector_insert();
  test_vector_inject();
  test_vector_append();
  test_vector_extend();
}
