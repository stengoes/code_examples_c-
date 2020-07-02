A minimal example for creating, compiling and using a static library in C++.

### 1. Create a test library

```c++
// test.h
#pragma once 
int add(int a, int b);
int mult(int a, int b);
```

```c++
// test.cc
int add(int a, int b){ return a+b; }
int mult(int a, int b){ return a*b; }
```

### 2. Compile the test library

```bash 
$ g++ -o test.o -c test.cc
```

### 3. Create static library by archiving all library obj files

```
$ ar -q libtest.a test.o
ar: creating archive libtest.a
```

### 4. Use the test library in a test project

```c++
\\ main.cc
#include <iostream>
#include "test.h"
int main()
{
    std::cout << "1 + 3 = " << add(1, 3) << std::endl;
    std::cout << "1 * 3 = " << mult(1, 3) << std::endl;
}
```

### 5. Compile the test project

```bash
$ g++ -o main libtest.a main.cc 
```

### 6. Run the test project

```bash
$ ./main
1 + 3 = 4
1 * 3 = 3
```