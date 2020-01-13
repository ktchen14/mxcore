#include <assert.h>
#include <dlfcn.h>
#include <errno.h>
#include <stdlib.h>

#include "../source/vector.h"

static int malloc_errno = 0;

void *malloc(size_t size) {
  typeof(malloc) *malloc = dlsym(RTLD_NEXT, "malloc");

  if (malloc_errno != 0)
    return errno = malloc_errno, NULL;
  return malloc(size);
}

#include <stdint.h>
#include <stdio.h>

int main() {
  int *vector;
  int data[] = { 1, 2, 3, 4 };
  // typedef struct _header_t {
  //   size_t volume;
  //   size_t length;
  //   char data[] __attribute__((aligned));
  // } header_t;

  // When volume * z overflows it allocates length
  // When volume * z + header size overflows it allocates the length
  // When allocation with volume is unsuccessful it alloates length

  // When the vector's data size when calculated from length overflows it
  // returns NULL
  // size_t length;
  // length = SIZE_MAX - sizeof(header_t) + 1;

  // With a length that, when a volume is calculated from it, causes the
  // vector's data size, when added to the header size, to overflow a size_t;
  // and when the length based allocation is unsuccessful; it returns NULL with
  // errno = ENOMEM

  // volume = (length * 8 + 3) / 5
  // length = (volume * 5 - 3) / 8
  // size_t volume = length / 5 * 8 + ((length % 5) * 8 + 3) / 5;

  size_t length;

  // Just length = (volume * 5 - 3) / 8 avoiding intermediate overflow
  length = SIZE_MAX / 8 * 5 + ((SIZE_MAX % 8) * 5 - 3) / 8 + 1;

  fprintf(stderr, "SIZE_MAX = %zu, length = %zu, volume = %zu\n", SIZE_MAX,
      length, length / 5 * 8 + ((length % 5) * 8 + 3) / 5);
  errno = 0;
  assert(mx_vector_create_with(data, length) == NULL);
  assert(errno == ENOMEM);

  // With a length that causes the vector's data size, when added to the header
  // size, to overflow a size_t; it returns NULL with errno = ENOMEM.
  length = SIZE_MAX / sizeof(int) - 1;
  errno = 0;
  assert(mx_vector_create_with(data, length) == NULL);
  assert(errno == ENOMEM);

  // When the allocation of the vector's data size is unsuccessful it returns
  // NULL with errno retained from malloc()
  malloc_errno = ENOENT;
  errno = 0;
  vector = mx_vector_create_with(data, sizeof(data) / sizeof(data[0]));
  assert(vector == NULL);
  assert(errno == ENOENT);
}
