#include <vector.h>

int main() {
  // It emits a compiler warning if its result is unused
  vector_resize((int *) vector_create(), 20);
}
