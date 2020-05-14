#ifndef TEST_H
#define TEST_H

#include <dlfcn.h>
#include <string.h>

#define REAL(name) ((__typeof__((name)) *) dlsym(RTLD_NEXT, #name))

// Assert that the data in and length of the vector is the same as the sequence
// of elements specified in the argument list. This uses memcmp() on the vector
// to determine equivalance.
#define assert_vector_data(vector, ...) do { \
  __typeof__((vector)) __vector = (vector); \
  \
  __typeof__(__vector[0]) __data[] = { __VA_ARGS__ }; \
  size_t __length = sizeof(__data) / sizeof(__data[0]); \
  \
  assert(vector_length(__vector) == __length); \
  assert(!memcmp(__vector, __data, sizeof(__data))); \
} while (0)

// Return the text of the argument in the source code itself as a C string
#define SOURCE(x) #x

// Return the text of the macro definition as a C string
#define DEFINITION_SOURCE(x) SOURCE(x)

#endif /* TEST_H */
