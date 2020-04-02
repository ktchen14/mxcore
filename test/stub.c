#include <stddef.h>
#include <stdlib.h>

void *stub_malloc(size_t size) {
  return malloc(size);
}

void *stub_realloc(void *data, size_t size) {
  return realloc(data, size);
}

void stub_free(void *data) {
  free(data);
}
