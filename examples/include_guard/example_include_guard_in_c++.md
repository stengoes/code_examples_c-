A minimal example for guarding against multiple includes in C++.

### The problem
```
TODO: explain multiple includes causing problmes when linking.
TODO: explain easyness of getting multiple inclusions with an include cycle.
```

### The solution
```c++
//test.h
#pragma once

// implementation of header file goes here.
```

### Alternative solution
```c++
//test.h
#ifndef TEST_H
#define TEST_H

// implementation of header file goes here.

#endif
```