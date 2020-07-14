
Implementation of a Timer class that asynchronously executes a callback function after a specified timeout has expired.

The class needs to implement functionality to start, stop or reset the timer (possibly with a new timeout). At construction one should also be able to opt to create an interval timer instead of a one-shot timer. 

### 1. Create the Timer class
```c++
//timer.h
#ifndef TIMER_H_
#define TIMER_H_

#include <thread>
class Timer
{
// class Timer is inspired by these posts:
// https://stackoverflow.com/questions/14650885/how-to-create-timer-events-using-c-11
// http://coliru.stacked-crooked.com/a/8804fee86d99a1e5

private:
  std::chrono::nanoseconds timeout;
  bool is_interval = false;
  std::function<void()> callback;

  std::thread timer_thread;
  std::condition_variable cv;
  std::mutex mutex;
  bool stop_signal = false;

  void thread()
  {
    std::unique_lock<std::mutex> lock(this->mutex);
    do{
      this->cv.wait_for(
          lock,
          this->timeout,
          [this] { return this->stop_signal; } // Predicate function to guard against spurious wake-up calls
      );

      if (this->stop_signal)
        return;
      else
        this->callback();
    } while(this->is_interval);
  }

public:
  // Use variadic template (a C++11 feature) that can handle a variable number ... of arguments
  template <typename Function, typename... Args>
  Timer(const std::chrono::nanoseconds timeout, const bool is_interval, Function &&func, Args &&... args)
  {
    this->timeout = timeout;
    this->is_interval = is_interval;

    // Change function signature to void() by binding arguments
    this->callback = std::function<void()>(std::bind(std::forward<Function>(func), std::forward<Args>(args)...));
  }
  void start()
  {
    this->timer_thread = std::thread(&Timer::thread, this);
  }
  void stop()
  {
    if (this->timer_thread.joinable())
    {
      {
        std::lock_guard<std::mutex> lock(this->mutex);
        this->stop_signal = true;
      }
      this->cv.notify_one();
      this->timer_thread.join();
      {
        std::lock_guard<std::mutex> lock(this->mutex);
        this->stop_signal = false;
      }
    }
  }
  void reset()
  {
    this->stop();
    this->start();
  }
  void reset(const std::chrono::nanoseconds timeout)
  {
    this->timeout = timeout;
    this->reset();
  }
  ~Timer()
  {
    this->stop();
  }
};

#endif
```

### 2. Create a test program
```c++
//main.cc
#include <iostream>
#include <thread> 

// Include the timer
#include "timer.h"

// Helper function
char *get_time()
{
  auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  return std::ctime(&now);
}

// Dummy function
int multiply(int a, int b)
{
  int answer = a * b;
  std::cout << get_time() << a << " * " << b << " = " << answer << std::endl;
  return answer;
}

int main()
{
  std::cout << get_time() << "Program start" << std::endl;

  // Create 3 timers
  Timer timer1(std::chrono::seconds(3), false, &multiply, 1, 5);
  Timer timer2(std::chrono::seconds(6), false, &multiply, 8, 5);
  Timer timer3(std::chrono::seconds(14), false, &multiply, 3, 17);

  // Start all timers
  timer1.start();
  timer2.start();
  timer3.start();

  // Reset timer2 after 4 seconds
  std::this_thread::sleep_for(std::chrono::seconds(4));
  timer2.reset();

  // Stop timer3 after another 8 seconds
  std::this_thread::sleep_for(std::chrono::seconds(8));
  timer3.stop();

  // Prevent program from exiting
  std::cin.get();
}
```
### 3. Compile the test program
```bash
g++ -std=c++17 -o main main.cc
```

### 4. Run the test progam
```bash
$ ./main
Tue Jul 14 21:53:30 2020
Program start
Tue Jul 14 21:53:33 2020
1 * 5 = 5
Tue Jul 14 21:53:40 2020
8 * 5 = 40
```

### 5. Explain the output
* Timer 1 will print "1 * 5 = 5" after 3 seconds.
* Timer 2 would print "8 * 5 = 40" after 6 seconds, but prints it after 10 seconds because of the reset after 4 seconds.
* Timer 3 would print "3 * 17 = 51" after 14 seconds, but is stopped after 12 seconds.

### Caveats
1. This code uses 1 thread per timer (not optimal when your applications needs a lot of timers).
2. This code does not set the thread priority (platform dependent code), which might be important if your timing needs to be critical.