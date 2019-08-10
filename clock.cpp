#include "clock.h"

void timer::start() {
	begin_time = std::chrono::steady_clock::now();
}

void timer::stop() {
	end_time = std::chrono::steady_clock::now();
}

float timer::time() {
	return std::chrono::duration_cast<std::chrono::milliseconds> (end_time - begin_time).count();
}
