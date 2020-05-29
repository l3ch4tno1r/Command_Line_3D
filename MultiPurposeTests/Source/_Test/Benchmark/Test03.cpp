#include <iostream>
#include <thread>

using namespace std::literals::chrono_literals;

#include "External/TheCherno/Instrumentor.h"

#include "Source/Utils/TimeMeasurement.h"

void Function()
{
	PROFILE_FUNC();

	std::cout << "Hello world !" << std::endl;

	std::this_thread::sleep_for(0.1s);
}

void Benchmark()
{
	PROFILE_FUNC();

	Function();
	Function();
	Function();
}

void Test()
{
	PROFILE_FUNC();

	Benchmark();
	Benchmark();
	Benchmark();
	Benchmark();
}

int FibonacciRecursive(int n)
{
	PROFILE_FUNC();

	if (n == 0 || n == 1)
		return 1;
	else
		return FibonacciRecursive(n - 1) + FibonacciRecursive(n - 2);
}

struct FibPair
{
	int x, y;
};

int FibonacciIterative(int n)
{
	PROFILE_FUNC();

	FibPair pair = { 1, 1 };

	for (int i = 0; i < n; ++i)
	{
		int x = pair.y;
		int y = pair.x + pair.y;

		pair = { x, y };
	}

	return pair.x;
}

int main()
{
	const int numiter = 20;

	Instrumentor::Get().BeginSession("TestLCN");

	{
		PROFILE_SCOPE("Test Profile");

		InstrumentationTimer timer("AUTO TEST");
	}

	{
		PROFILE_SCOPE("FibonacciRecursive");

		for (int i = 0; i < numiter; i++)
			std::cout << FibonacciRecursive(i) << std::endl;
	}

	std::cout << "----------------" << std::endl;

	{
		PROFILE_SCOPE("FibonacciIterative");

		for (int i = 0; i < numiter; i++)
			std::cout << FibonacciIterative(i) << std::endl;
	}

	std::cin.get();

	Instrumentor::Get().EndSession();

}