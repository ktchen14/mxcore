#include <assert.h>
#include <dlfcn.h>
#include <errno.h>
#include <stdlib.h>

#include "../source/vector.h"
#include "test.h"

static int ensure_errno = 0;
mx_vector_t mx_vector_ensure_z(mx_vector_t vector, size_t length, size_t z) {
  typeof(mx_vector_ensure_z) *mx_vector_ensure_z =
    dlsym(RTLD_NEXT, "mx_vector_ensure_z");
  if (ensure_errno != 0)
    return errno = ensure_errno, NULL;
  return mx_vector_ensure_z(vector, length, z);
}

static size_t last_insert_z;
mx_vector_t mx_vector_insert_z(
    mx_vector_t vector, size_t i, const void *elmt, size_t z) {
  typeof(mx_vector_insert_z) *mx_vector_insert_z =
    dlsym(RTLD_NEXT, "mx_vector_insert_z");
  last_insert_z = z;
  return mx_vector_insert_z(vector, i, elmt, z);
}

static mx_vector_t last_vector;
static size_t last_i;
static const void *last_elmt;
static size_t last_n;
static size_t last_inject_z;
static mx_vector_t last_result;
mx_vector_t mx_vector_inject_z(
    mx_vector_t vector, size_t i, const void *elmt, size_t n, size_t z) {
  typeof(mx_vector_inject_z) *mx_vector_inject_z =
    dlsym(RTLD_NEXT, "mx_vector_inject_z");

  last_vector = vector;
  last_i = i;
  last_elmt = elmt;
  last_n = n;
  last_inject_z = z;

  last_result = mx_vector_inject_z(vector, i, elmt, n, z);

  return last_result;
}

static size_t last_append_z;
mx_vector_t mx_vector_append_z(mx_vector_t vector, const void *elmt, size_t z) {
  typeof(mx_vector_append_z) *mx_vector_append_z =
    dlsym(RTLD_NEXT, "mx_vector_append_z");
  last_append_z = z;
  return mx_vector_append_z(vector, elmt, z);
}

static size_t last_extend_z;
mx_vector_t mx_vector_extend_z(
    mx_vector_t vector, const void *elmt, size_t n, size_t z) {
  typeof(mx_vector_extend_z) *mx_vector_extend_z =
    dlsym(RTLD_NEXT, "mx_vector_extend_z");
  last_extend_z = z;
  return mx_vector_extend_z(vector, elmt, n, z);
}

void test_vector_insert(void) {
  int *vector = mx_vector_define(int, 1, 2, 3, 5, 8, 13);
  int data = 13;
  int number = 0;

  // It evaluates its vector argument once
  vector = mx_vector_insert((number++, vector), 2, &data);
  assert(number == 1);

  // It evaluates its index argument once
  vector = mx_vector_insert(vector, (number++, 2), &data);
  assert(number == 2);

  // It evaluates its element argument once
  vector = mx_vector_insert(vector, 2, (number++, &data));
  assert(number == 3);

  // It calls mx_vector_insert_z() with the element size of the vector
  vector = mx_vector_insert(vector, 2, &data);
  assert(last_insert_z == sizeof(vector[0]));

  // It delegates to mx_vector_inject_z() with length as 1
  int *result = mx_vector_insert(vector, 2, &data);
  assert(last_vector == vector);
  assert(last_i == 2);
  assert(last_elmt == &data);
  assert(last_n == 1);
  assert(last_inject_z == sizeof(vector[0]));
  assert(result == last_result);

  mx_vector_delete(vector);
}

void test_vector_inject(void) {
  int *vector = mx_vector_define(int, 0, 1, 2, 3, 4, 5, 6, 7);
  int *result;
  int data[] = { 9, 11 };
  size_t data_length = sizeof(data) / sizeof(data[0]);
  int number = 0;

  // It evaluates its vector argument once
  vector = mx_vector_inject((number++, vector), 2, &data, data_length);
  assert(number == 1);

  // It evaluates its index argument once
  vector = mx_vector_inject(vector, (number++, 2), &data, data_length);
  assert(number == 2);

  // It evaluates its element argument once
  vector = mx_vector_inject(vector, 2, (number++, &data), data_length);
  assert(number == 3);

  // It evaluates its length argument once
  vector = mx_vector_inject(vector, 2, &data, (number++, data_length));
  assert(number == 4);

  // It calls mx_vector_inject_z() with the element size of the vector
  vector = mx_vector_inject(vector, 2, &data, data_length);
  assert(last_inject_z == sizeof(vector[0]));

  mx_vector_delete(vector);

  // With a length that, when added to the vector's length, overflows a size_t;
  // it returns NULL with errno = ENOMEM. The vector is unmodified.
  vector = mx_vector_define(int, 1, 2, 3, 5);

  int maximum_length = SIZE_MAX - mx_vector_length(vector);
  errno = 0;
  result = mx_vector_inject(vector, 2, &data, maximum_length + 1);
  assert(result == NULL);
  assert(errno == ENOMEM);

  assert_vector_data(vector, 1, 2, 3, 5);

  // When the ensure operation is unsuccessful it returns NULL with errno
  // retained from mx_vector_ensure(). The vector is unmodified.
  ensure_errno = ENOENT;
  errno = 0;
  result = mx_vector_inject(vector, 2, &data, data_length);
  ensure_errno = 0;

  assert(result == NULL);
  assert(errno == ENOENT);

  assert_vector_data(vector, 1, 2, 3, 5);

  // When the element isn't NULL it injects length elements into the vector at
  // the index from the element location
  vector = mx_vector_inject(vector, 2, &data, data_length);
  assert_vector_data(vector, 1, 2, 9, 11, 3, 5);

  // When the element is NULL it injects length uninitialized elements into the
  // vector at the index
  vector = mx_vector_inject(vector, 4, NULL, 1);
  assert_vector_data(vector, 1, 2, 9, 11, 3, 3, 5);

  // When the index is equal to the vector's length it appends length elements
  // onto the vector
  vector = mx_vector_inject(vector, mx_vector_length(vector), &data, 2);
  assert_vector_data(vector, 1, 2, 9, 11, 3, 3, 5, 9, 11);

  mx_vector_delete(vector);
}

void test_vector_append(void) {
  int *vector = mx_vector_define(int, 1, 2, 3, 5, 8, 13);
  int data = 13;
  int number = 0;

  // It evaluates its vector argument once
  vector = mx_vector_append((number++, vector), &data);
  assert(number == 1);

  // It evaluates its element argument once
  vector = mx_vector_append(vector, (number++, &data));
  assert(number == 2);

  // It calls mx_vector_append_z() with the element size of the vector
  vector = mx_vector_append(vector, &data);
  assert(last_append_z == sizeof(vector[0]));

  // It delegates to mx_vector_inject_z() with the length and element size of
  // the vector
  size_t length = mx_vector_length(vector);
  int *result = mx_vector_append(vector, &data);
  assert(last_vector == vector);
  assert(last_i == length);
  assert(last_elmt == &data);
  assert(last_n == 1);
  assert(last_inject_z == sizeof(vector[0]));
  assert(result == last_result);

  mx_vector_delete(vector);
}

void test_vector_extend(void) {
  int *vector = mx_vector_define(int, 1, 2, 3, 5, 8, 13);
  int data[] = { 9, 11 };
  size_t data_length = sizeof(data) / sizeof(data[0]);
  int number = 0;

  // It evaluates its vector argument once
  vector = mx_vector_extend((number++, vector), &data, data_length);
  assert(number == 1);

  // It evaluates its element argument once
  vector = mx_vector_extend(vector, (number++, &data), data_length);
  assert(number == 2);

  // It evaluates its length argument once
  vector = mx_vector_extend(vector, &data, (number++, data_length));
  assert(number == 3);

  // It calls mx_vector_extend_z() with the element size of the vector
  vector = mx_vector_extend(vector, &data, data_length);
  assert(last_extend_z == sizeof(vector[0]));

  // It delegates to mx_vector_inject_z() with the length and element size of
  // the vector
  size_t length = mx_vector_length(vector);
  int *result = mx_vector_extend(vector, &data, data_length);
  assert(last_vector == vector);
  assert(last_i == length);
  assert(last_elmt == &data);
  assert(last_n == data_length);
  assert(last_inject_z == sizeof(vector[0]));
  assert(result == last_result);

  mx_vector_delete(vector);
}

int main() {
  test_vector_insert();
  test_vector_inject();
  test_vector_append();
  test_vector_extend();
}
