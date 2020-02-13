#pragma once

#include <iostream>
#include <chrono>

using namespace std;

#define MEASURETIME(X)\
{\
	auto _start = chrono::high_resolution_clock::now();\
	X;\
	auto _end = chrono::high_resolution_clock::now();\
	long long _ellapsed_micros = chrono::duration_cast<chrono::microseconds> (_end - _start).count();\
	cout << "Execution time of '" << #X << "' : " << (float)_ellapsed_micros / 1000.0f << " ms" << endl;\
}

class Timer
{
private:
	chrono::steady_clock::time_point start;

public:
	Timer();

	~Timer();
};