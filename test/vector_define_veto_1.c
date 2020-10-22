#include <vector.h>

int main() {
  // It emits a compiler error when its type argument isn't an actual type
  int *vector = vector_define(1, 2, 3, 4);
}
