#include <assert.h>
#include <stdalign.h>
#include <stddef.h>
#include <stdint.h>

#include <vector.h>

void test_vector_header(void) {
  // Its alignment requirement is as strict as that of a max_align_t
  assert(alignof(struct __vector_header_t) >= alignof(max_align_t));

  // Its data member's offset is aligned to a max_align_t
  size_t offset = offsetof(struct __vector_header_t, data);
  assert(offset % alignof(max_align_t) == 0);

  // The result of vector_create() is aligned to a max_align_t
  int *source = vector_create();
  assert((uintptr_t) source % alignof(max_align_t) == 0);

  // The result of vector_duplicate() is aligned to a max_align_t
  int *target = vector_duplicate(source);
  assert((uintptr_t) target % alignof(max_align_t) == 0);

  vector_delete(source);
  vector_delete(target);

  // The result of vector_import() is aligned to a max_align_t
  int data[] = { 1, 2, 3, 5, 8, 13 };
  int *vector = vector_import(data, sizeof(data) / sizeof(data[0]));
  assert((uintptr_t) vector % alignof(max_align_t) == 0);

  // The result of vector_resize() is aligned to a max_align_t
  vector = vector_resize(vector, vector_volume(vector) + 20);
  assert((uintptr_t) vector % alignof(max_align_t) == 0);

  vector_delete(vector);
}

int main() {
  test_vector_header();
}
