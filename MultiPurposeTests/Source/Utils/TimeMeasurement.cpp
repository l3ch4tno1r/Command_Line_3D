#include "TimeMeasurement.h"
#include "ErrorHandling.h"

#include <sstream>

Timer::Timer()
{
	start = std::chrono::high_resolution_clock::now();
}

Timer::~Timer()
{
	auto end = std::chrono::high_resolution_clock::now();
	long long ellapsed_millis = std::chrono::duration_cast<std::chrono::milliseconds> (end - start).count();

	ASSERT(ellapsed_millis < 16);
	std::cout << "Block execution time : " << (float)ellapsed_millis / 1000.0f << " ms" << std::endl;
}

Timer2::Timer2(std::string & _target) :
	str(_target),
	start(std::chrono::high_resolution_clock::now())
{}

Timer2::~Timer2()
{
	auto end = std::chrono::high_resolution_clock::now();
	long long ellapsed_micros = std::chrono::duration_cast<std::chrono::microseconds> (end - start).count();

	std::stringstream sstr;

	sstr << "Block execution time : " << (float)ellapsed_micros / 1000.0f << " ms";

	str = sstr.str();
}
