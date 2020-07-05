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
  Timer timer1(std::chrono::seconds(3), &multiply, 1, 5);
  Timer timer2(std::chrono::seconds(6), &multiply, 8, 5);
  Timer timer3(std::chrono::seconds(14), &multiply, 3, 17);

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

// Program output:
// Timer 1 will print "1 * 5 = 5" after 3 seconds.
// Timer 2 would print "8 * 5 = 40" after 6 seconds, 
// but prints it after 10 seconds because of the reset after 4 seconds.
// Timer 3 would print "3 * 17 = 51" after 14 seconds, but is stopped after 12 seconds.
