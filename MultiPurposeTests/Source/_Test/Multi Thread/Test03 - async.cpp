#include <iostream>
#include <future>

#include "Source/Utils/TraceLog.h"

using namespace LCNUtilities;
using namespace std::literals::chrono_literals;

int ExessivelyLongAddition(int a, int b)
{
	std::this_thread::sleep_for(2s);

	return a + b;
}

int main()
{
	Log() << "Main Begin";

	std::future<int> f1 = std::async(std::launch::async, ExessivelyLongAddition, 1, 2);
	std::future<int> f2 = std::async(std::launch::async, ExessivelyLongAddition, 3, 4);
	std::future<int> f3 = std::async(std::launch::async, ExessivelyLongAddition, 5, 6);

	Log() << "Main Continue";

	Log() << f1.get() << ", " << f2.get() << ", " << f3.get();

	Log() << "Main End";

	std::cin.get();
}