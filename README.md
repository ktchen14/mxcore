Vector
======

[![CI status](https://ktchen14.semaphoreci.com/badges/mxcore.svg)](https://ktchen14.semaphoreci.com/projects/mxcore)
[![Code Coverage](https://codecov.io/gh/ktchen14/mxcore/branch/master/graph/badge.svg)](https://codecov.io/gh/ktchen14/mxcore)

Introduction
------------

This is an implementation of vectors, or [dynamic
arrays](https://en.wikipedia.org/wiki/Dynamic_array), for C. A `vector` in this
library behaves like a normal array when its dynamic allocation properties
aren't being used. For example:

```c
#include <vector.h>

int main(int argc, char *argv[]) {
  int *vector = vector_define(int, 1, 2, 3, 4);
  vector = vector_extend(vector, (int[]) { 5, 6, 7, 8 }, 4);

  for (size_t i = 0; i < vector_length(vector); i++)
    printf("%i ", vector[i]);
}
```

Output:

```
1 2 3 4 5 6 7 8
```

Implementation
--------------

Installation
------------

```sh
git clone --branch stable https://github.com/ktchen14/vector.git
cd vector
autoreconf --install
./configure
make
make install
```
