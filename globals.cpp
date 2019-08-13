#include <thread>

#include "globals.h"

int thread_count;
bool debug_mode;

void init_vars() {
	// Set default thread count
	thread_count = std::thread::hardware_concurrency() - 1;

	// Set debug_mode default
	debug_mode = true;
}

