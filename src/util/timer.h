#ifndef Timer_h
#define Timer_h

#include <chrono>

class Timer {
  public:
	static std::chrono::time_point<std::chrono::high_resolution_clock> now();
};

#endif  /* Timer_h */
