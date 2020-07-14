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