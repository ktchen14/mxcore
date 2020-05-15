#include <stdbool.h>
#include <assert.h>

#include <vector.h>
#include "test.h"

static bool eqintlongp(const void *a, const void *b) {
  return *(const int *) a == *(const long *) b;
}

static int counter = 0;
static bool assert_counter_zero(const void *a, const void *b) {
  (void) a;
  (void) b;
  assert(counter == 0);
  return false;
}

// vector_eq(), vector_eq_z()

static size_t last_eq_za, last_eq_zb;
bool vector_eq_z(
    vector_c va,
    vector_c vb,
    bool (*eq)(const void *a, const void *b),
    size_t za,
    size_t zb) {
  last_eq_za = za;
  last_eq_zb = zb;
  return REAL(vector_eq_z)(va, vb, eq, za, zb);
}

void test_vector_eq(void) {
  int *va = vector_define(int, 1, 2, 3, 5, 8, 13);
  long *vb = vector_define(long, 1, 2, 3, 5, 8, 13);
  int *vector;
  bool result __attribute__((unused));
  int number = 0;

  // It evaluates each argument once
  result = vector_eq((number++, va), vb, eqintlongp);
  assert(number == 1);
  result = vector_eq(va, (number++, vb), eqintlongp);
  assert(number == 2);
  result = vector_eq(va, vb, (number++, eqintlongp));
  assert(number == 3);

  // It calls vector_eq_z() with the element sizes of the vectors
  result = vector_eq(va, vb, eqintlongp);
  assert(last_eq_za == sizeof(va[0]));
  assert(last_eq_zb == sizeof(vb[0]));

  // Its expansion is an expression
  assert(vector_eq(va, vb, eqintlongp));

  // When both va and vb are NULL it returns true
  assert(vector_eq((int *) NULL, (long *) NULL, eqintlongp));

  // When va isn't NULL and vb is NULL then it returns false
  assert(!vector_eq(va, (long *) NULL, eqintlongp));

  // When va is NULL and vb isn't NULL then it returns false
  assert(!vector_eq((int *) NULL, vb, eqintlongp));

  // When the lengths of va and vb differ it returns false before calling eq
  vector = vector_define(int, 1, 2, 3, 5);
  counter = 1;
  assert(!vector_eq(va, vector, assert_counter_zero));
  vector_delete(vector);

  // When neither va or vb is NULL it returns whether eq returns true when
  // called on each element in va and its mirror element in vb
  assert(vector_eq(va, vb, eqintlongp));

  // When an element in va isn't equivalent to its mirror element in vb it
  // returns false before calling eq on the next pair of elements
  counter = 0;
  assert(!vector_eq(va, vb, assert_counter_zero));

  vector_delete(va);
  vector_delete(vb);
}

void test_vector_eq_with(void) {
  int *va = vector_define(int, 1, 2, 3, 5, 8, 13);
  long *vb = vector_define(long, 1, 2, 3, 5, 8, 13);
  int *vector;
  bool result __attribute__((unused));
  int number = 0;

  // TODO: assert(false);

  // It evaluates each argument once
  result = vector_eq((number++, va), vb, eqintlongp);
  assert(number == 1);
  result = vector_eq(va, (number++, vb), eqintlongp);
  assert(number == 2);
  result = vector_eq(va, vb, (number++, eqintlongp));
  assert(number == 3);

  // It calls vector_eq_z() with the element sizes of the vectors
  result = vector_eq(va, vb, eqintlongp);
  assert(last_eq_za == sizeof(va[0]));
  assert(last_eq_zb == sizeof(vb[0]));

  // Its expansion is an expression
  assert(vector_eq(va, vb, eqintlongp));

  // When both va and vb are NULL it returns true
  assert(vector_eq((int *) NULL, (long *) NULL, eqintlongp));

  // When va isn't NULL and vb is NULL then it returns false
  assert(!vector_eq(va, (long *) NULL, eqintlongp));

  // When va is NULL and vb isn't NULL then it returns false
  assert(!vector_eq((int *) NULL, vb, eqintlongp));

  // When the lengths of va and vb differ it returns false before calling eq
  vector = vector_define(int, 1, 2, 3, 5);
  counter = 1;
  assert(!vector_eq(va, vector, assert_counter_zero));
  vector_delete(vector);

  // When neither va or vb is NULL it returns whether eq returns true when
  // called on each element in va and its mirror element in vb
  assert(vector_eq(va, vb, eqintlongp));

  // When an element in va isn't equivalent to its mirror element in vb it
  // returns false before calling eq on the next pair of elements
  counter = 0;
  assert(!vector_eq(va, vb, assert_counter_zero));

  vector_delete(va);
  vector_delete(vb);
}

int main() {
  test_vector_eq();
}
