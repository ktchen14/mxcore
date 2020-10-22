#include <assert.h>
#include <errno.h>
#include <stdlib.h>

#include <vector.h>

// TODO: review

static int malloc_errno = 0;
__attribute__((used)) void *stub_malloc(size_t size) {
  if (malloc_errno != 0)
    return errno = malloc_errno, NULL;
  return malloc(size);
}

int main() {
  int number = 0;

  // It evaluates each argument once
  vector_delete(vector_define(int, (number++, 1), 2, 3, 5));
  assert(number == 1);
  vector_delete(vector_define(int, 1, (number++, 2), 3, 5));
  assert(number == 2);
  vector_delete(vector_define(int, 1, 2, (number++, 3), 5));
  assert(number == 3);
  vector_delete(vector_define(int, 1, 2, 3, (number++, 5)));
  assert(number == 4);

  // Its expansion is an expression
  assert(vector_delete(vector_define(int, 1)) == NULL);

  // When the allocation is unsuccessful it returns NULL with errno retained
  // from malloc()
  malloc_errno = ENOENT;
  errno = 0;
  assert(vector_define(int, 1, 2) == NULL);
  assert(errno == ENOENT);

  malloc_errno = 0;

  // It accepts a scalar type as its type argument and type compatible scalar
  // literals as its variadic arguments
  int *vector_i = vector_define(int, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11);

  assert(vector_i != NULL);
  assert(vector_volume(vector_i) >= vector_length(vector_i));
  assert(vector_length(vector_i) == 12);
  for (size_t i = 0; i < vector_length(vector_i); i++)
    assert(vector_i[i] == (int) i);

  vector_delete(vector_i);

  // It accepts an array of constant known size as its type argument, and type
  // and size compatible array literals (brace enclosed lists of initializers)
  // as its variadic arguments.
  int (*vector_a)[2] = vector_define(int[2], { 2, 3 }, { 5, 8 });

  assert(vector_a != NULL);
  assert(vector_volume(vector_a) >= vector_length(vector_a));
  assert(vector_length(vector_a) == 2);
  assert(vector_a[0][0] == 2 && vector_a[0][1] == 3);
  assert(vector_a[1][0] == 5 && vector_a[1][1] == 8);

  vector_delete(vector_a);

  // It accepts a pointer to a VLA as its type argument and pointers to type
  // compatible VLAs as its variadic arguments. Its type argument is evaluated
  // once.
  int length = 2, va[length], vb[length];

  int v = 1;
  int (**vector_v)[length] = vector_define(int(*)[v += 1], &va, &vb);
  assert(v == 2);

  assert(vector_v != NULL);
  assert(vector_volume(vector_v) >= vector_length(vector_v));
  assert(vector_length(vector_v) == 2);
  assert(vector_v[0] == &va);
  assert(vector_v[1] == &vb);

  vector_delete(vector_v);

  // It accepts a struct declaration as its type argument and compatible struct
  // literals (brace enclosed lists of initializers) as its variadic arguments
  struct { long a; int b; } *vector_s = vector_define(
    struct { long a; int b; }, { .a = 2, .b = 3 }, { .a = 5, .b = 8 },
  );

  assert(vector_s != NULL);
  assert(vector_volume(vector_s) >= vector_length(vector_s));
  assert(vector_length(vector_s) == 2);
  assert(vector_s[0].a == 2 && vector_s[0].b == 3);
  assert(vector_s[1].a == 5 && vector_s[1].b == 8);

  vector_delete(vector_s);

  // It accepts a union declaration as its type argument and compatible union
  // literals (brace enclosed lists of initializers) as its variadic arguments
  union { long a; void *b; } *vector_u = vector_define(
    union { long a; void *b; }, { .a = 8 }, { .b = &vector_u },
  );

  assert(vector_u != NULL);
  assert(vector_volume(vector_u) >= vector_length(vector_u));
  assert(vector_length(vector_u) == 2);
  assert(vector_u[0].a == 8);
  assert(vector_u[1].b == &vector_u);

  vector_delete(vector_u);

  // It accepts a pointer to a scalar type as its type argument and type
  // compatible pointers as its variadic arguments
  int pa, pb;
  int **vector_p = vector_define(int*, &pa, &pb);

  assert(vector_p != NULL);
  assert(vector_volume(vector_p) >= vector_length(vector_p));
  assert(vector_length(vector_p) == 2);
  assert(vector_p[0] == &pa);
  assert(vector_p[1] == &pb);

  vector_delete(vector_p);

  // It rejects an incomplete type
  // TODO
}
