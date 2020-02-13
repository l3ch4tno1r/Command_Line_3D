#include "TimeMeasurement.h"

Timer::Timer()
{
	start = chrono::high_resolution_clock::now();
}

Timer::~Timer()
{
	auto end = chrono::high_resolution_clock::now();
	long long ellapsed_micros = chrono::duration_cast<chrono::microseconds> (end - start).count();
	cout << "Block execution time : " << (float)ellapsed_micros / 1000.0f << " ms" << endl;
}
