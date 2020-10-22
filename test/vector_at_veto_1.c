#include <vector.h>

int main() {
  // It emits a compiler warning if its vector argument isn't a pointer
  vector_at(1, 0, sizeof(size_t));
}
