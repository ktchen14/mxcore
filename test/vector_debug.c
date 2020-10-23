#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>

#include <vector.h>
#include "test.h"

static int backup_stderr;

static FILE *redirect_stderr(void) {
  backup_stderr = dup(STDERR_FILENO);
  FILE *stream = tmpfile();
  setbuf(stream, NULL);
  dup2(fileno(stream), STDERR_FILENO);
  return stream;
}

static void undirect_stderr(FILE *stream, char *buffer, size_t size) {
  fseek(stream, 0, SEEK_SET);
  size_t length = fread(buffer, 1, size - 1, stream);
  fclose(stream);
  buffer[length] = '\0';

  dup2(backup_stderr, STDERR_FILENO);
  close(backup_stderr);
}

static size_t last_debug_z;
void vector_debug_z(
    vector_c vector, void (*elmt_debug)(const void *elmt), size_t z) {
  REAL(vector_debug_z)(vector, elmt_debug, last_debug_z = z);
}

void debugintp(const void *elmt) {
  fprintf(stderr, "+%i", *(const int *) elmt);
}

void test_vector_debug(void) {
  int *vector = vector_define(int, 1, 2, 3, 5);
  int number = 0;
  FILE *stderr_stream;
  char buffer[4096];

  // It evaluates each argument once
  vector_debug((number++, vector), debugintp);
  assert(number == 1);
  vector_debug(vector, (number++, debugintp));
  assert(number == 2);

  // It calls vector_debug_z() with the element size of the vector
  vector_debug(vector, debugintp);
  assert(last_debug_z == sizeof(vector[0]));

  // Its expansion is an expression
  assert((vector_debug(vector, debugintp), 1));

  // It prints debugging information about each element in the vector to stderr
  stderr_stream = redirect_stderr();
  vector_debug(vector, debugintp);
  undirect_stderr(stderr_stream, buffer, sizeof(buffer));
  assert(!strcmp(buffer, "[+1, +2, +3, +5]\n"));

  // With an empty vector it prints "[]" to stderr
  vector = vector_truncate(vector, 0);

  stderr_stream = redirect_stderr();
  vector_debug(vector, debugintp);
  undirect_stderr(stderr_stream, buffer, sizeof(buffer));
  assert(!strcmp(buffer, "[]\n"));

  vector_delete(vector);
}

int main() {
  test_vector_debug();
}
