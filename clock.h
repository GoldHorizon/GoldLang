#pragma once

#include <chrono>

namespace timer {
	void start();
	void stop();

	float time();

	static std::chrono::steady_clock::time_point begin_time;
	static std::chrono::steady_clock::time_point end_time;
};
