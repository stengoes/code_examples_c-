A minimal example for guarding against multiple includes in C++.

### The problem
```
TODO: explain multiple includes causing problmes when linking.
TODO: explain easyness of getting multiple inclusions with an include cycle.
```

### The solution

```c++
// path/to/test.h
#ifndef PATH_TO_TEST_H_
#define PATH_TO_TEST_H_

// ... implementation of header file goes here.

#endif
```

### Alternative solution
```c++
// path/to/test.h
#pragma once

// ... implementation of header file goes here.
```

#### Disadvantages:
* Not part of the C++ ISO standard (though widely supported by )

#### Advantages:
* Less code than include guard
* Less prone to bugs
* No name clashes
* Faster compile times (for some compilers)

## References:
1. [Google C++ Style Guide: the #define Guard](https://google.github.io/styleguide/cppguide.html#The__define_Guard)
2. [Stack Overflow: #pragma once vs include guards? [duplicate]](https://stackoverflow.com/questions/1143936/pragma-once-vs-include-guards)
3. [Wikipedia: #pragma once](https://en.wikipedia.org/wiki/Pragma_once)
4. [Wikipedia: include guard](https://en.wikipedia.org/wiki/Include_guard)