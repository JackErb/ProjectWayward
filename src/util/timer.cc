#include "timer.h"
#include <chrono>

using namespace std::chrono;

time_point<high_resolution_clock> Timer::now() {
	return high_resolution_clock::now();
}
