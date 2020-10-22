Vector
======

[![CI status](https://ktchen14.semaphoreci.com/badges/mxcore.svg)](https://ktchen14.semaphoreci.com/projects/mxcore)
[![Code Coverage](https://codecov.io/gh/ktchen14/mxcore/branch/master/graph/badge.svg)](https://codecov.io/gh/ktchen14/mxcore)

[API Documentation](https://mxcore.readthedocs.io/en/stable/)

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

Most functions that change the size of the vector return the resultant vector.

The element size or type isn't stored in the vector. Most of the time the
"implicit interface" should be used. All implicit interface functions calculate
the element size of the vector from its type. An alternative "explicit
interface" is available where the element size of the vector needs to be passed
in. These can be mixed freely:

```c
int *vector = vector_create();
int number = 1;
vector = vector_append(vector, &number);

number = 2;
vector = vector_append_z(vector, &number, sizeof(int));
```

Almost all functions in the "explicit interface" have a `_z` suffix.

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
