A minimal example for guarding against multiple includes in C++.

### The problem
```
TODO: explain that including the same definition multiple times, causes problmes when linking.
TODO: explain why including the same header file multiple times is not as dumb of a mistake as it may first seem. Picture an include cycle for example.
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
* Not part of the C++ ISO standard, though widely supported by almost all compilers.
* It is not part of the standard, because it cannot handle symbolic linking of header files properly.

#### Advantages:
* Less code than include guard (it is only 1 line).
* Therefore it is less prone to typos.
* No need to come up with unique naming convention, hence no name clashes (especially important in big projects).
* Faster compile times for some compilers (not really).

## Conclusion
I use both solutions. 

## References:
1. [Google C++ Style Guide: the #define Guard](https://google.github.io/styleguide/cppguide.html#The__define_Guard)
2. [Stack Overflow: #pragma once vs include guards? [duplicate]](https://stackoverflow.com/questions/1143936/pragma-once-vs-include-guards)
3. [Wikipedia: #pragma once](https://en.wikipedia.org/wiki/Pragma_once)
4. [Wikipedia: include guard](https://en.wikipedia.org/wiki/Include_guard)