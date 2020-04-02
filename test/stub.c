#include <stddef.h>
#include <stdlib.h>

void *stub_malloc(size_t size) {
  return malloc(size);
}

void stub_free(void *data) {
  free(data);
}
