#include <assert.h>
#include <dlfcn.h>
#include <errno.h>
#include <stdlib.h>

#include "../source/vector.h"

// TODO: review

static int malloc_errno = 0;
void *malloc(size_t size) {
  typeof(malloc) *malloc = dlsym(RTLD_NEXT, "malloc");

  if (malloc_errno != 0)
    return errno = malloc_errno, NULL;
  return malloc(size);
}

int main() {
  // When the allocation is unsuccessful it returns NULL with errno retained
  // from malloc()
  malloc_errno = ENOENT;
  errno = 0;
  assert(mx_vector_define(int, 1, 2) == NULL);
  assert(errno == ENOENT);

  malloc_errno = 0;

  // It accepts a scalar type as its type argument and type compatible scalar
  // literals as its variadic arguments
  int *vector_i = mx_vector_define(int, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11);

  assert(vector_i != NULL);
  assert(mx_vector_volume(vector_i) >= mx_vector_length(vector_i));
  assert(mx_vector_length(vector_i) == 12);
  for (size_t i = 0; i < mx_vector_length(vector_i); i++)
    assert(vector_i[i] == (int) i);

  mx_vector_delete(vector_i);

  // It accepts an array of constant known size as its type argument, and type
  // and size compatible array literals (brace enclosed lists of initializers)
  // as its variadic arguments.
  int (*vector_a)[2] = mx_vector_define(int[2], { 1, 2 }, { 3, 4 });

  assert(vector_a != NULL);
  assert(mx_vector_volume(vector_a) >= mx_vector_length(vector_a));
  assert(mx_vector_length(vector_a) == 2);
  assert(vector_a[0][0] == 1 && vector_a[0][1] == 2);
  assert(vector_a[1][0] == 3 && vector_a[1][1] == 4);

  mx_vector_delete(vector_a);

  // It accepts a pointer to a VLA as its type argument and pointers to type
  // compatible VLAs as its variadic arguments. Its type argument is evaluated
  // once.
  int length = 2, va[length], vb[length];

  int v = 1;
  int (**vector_v)[length] = mx_vector_define(int(*)[v += 1], &va, &vb);
  assert(v == 2);

  assert(vector_v != NULL);
  assert(mx_vector_volume(vector_v) >= mx_vector_length(vector_v));
  assert(mx_vector_length(vector_v) == 2);
  assert(vector_v[0] == &va);
  assert(vector_v[1] == &vb);

  mx_vector_delete(vector_v);

  // It accepts a struct declaration as its type argument and compatible struct
  // literals (brace enclosed lists of initializers) as its variadic arguments
  struct { long a; int b; } *vector_s = mx_vector_define(
    struct { long a; int b; }, { .a = 1, .b = 2 }, { .a = 3, .b = 4 },
  );

  assert(vector_s != NULL);
  assert(mx_vector_volume(vector_s) >= mx_vector_length(vector_s));
  assert(mx_vector_length(vector_s) == 2);
  assert(vector_s[0].a == 1 && vector_s[0].b == 2);
  assert(vector_s[1].a == 3 && vector_s[1].b == 4);

  mx_vector_delete(vector_s);

  // It accepts a union declaration as its type argument and compatible union
  // literals (brace enclosed lists of initializers) as its variadic arguments
  union { long a; void *b; } *vector_u = mx_vector_define(
    union { long a; void *b; }, { .a = 1 }, { .b = &vector_u },
  );

  assert(vector_u != NULL);
  assert(mx_vector_volume(vector_u) >= mx_vector_length(vector_u));
  assert(mx_vector_length(vector_u) == 2);
  assert(vector_u[0].a == 1);
  assert(vector_u[1].b == &vector_u);

  mx_vector_delete(vector_u);

  // It accepts a pointer to a scalar type as its type argument and type
  // compatible pointers as its variadic arguments
  int pa, pb;
  int **vector_p = mx_vector_define(int*, &pa, &pb);

  assert(vector_p != NULL);
  assert(mx_vector_volume(vector_p) >= mx_vector_length(vector_p));
  assert(mx_vector_length(vector_p) == 2);
  assert(vector_p[0] == &pa);
  assert(vector_p[1] == &pb);

  mx_vector_delete(vector_p);

  // It rejects an incomplete type

#ifdef MAKE_TEST_WILL_FAIL
  // It emits a compiler error when its type argument isn't an actual type
  mx_vector_define(1, 2, 3, 4);
#endif /* MAKE_TEST_WILL_FAIL */
}
