#include <iostream>
#include <thread>

using namespace std::literals::chrono_literals;

/*
#include "External/TheCherno/Instrumentor.h"
*/
#include "Source/Utils/Instrumentor.h"

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
		PROFILE_SCOPE("Test Profile 1");

		InstrumentationTimer timer("AUTO TEST");
	}

	/*
	{
		PROFILE_SCOPE("FibonacciRecursive");

		for (int i = 0; i < numiter; i++)
			std::cout << FibonacciRecursive(i) << std::endl;
	}

	std::cout << "----------------" << std::endl;

	{
		PROFILE_SCOPE("FibonacciIterative - for loop");

		for (int i = 0; i < numiter; i++)
			FibonacciIterative(i);
	}

	{
		PROFILE_SCOPE("FibonacciIterative");

		FibonacciIterative(0);
		FibonacciIterative(1);
		FibonacciIterative(2);
		FibonacciIterative(3);
		FibonacciIterative(4);
		FibonacciIterative(5);
		FibonacciIterative(6);
		FibonacciIterative(7);
		FibonacciIterative(8);
		FibonacciIterative(9);
		FibonacciIterative(10);
		FibonacciIterative(11);
		FibonacciIterative(12);
		FibonacciIterative(13);
		FibonacciIterative(14);
		FibonacciIterative(15);
		FibonacciIterative(16);
		FibonacciIterative(17);
		FibonacciIterative(18);
		FibonacciIterative(19);
	}
	*/

	std::cin.get();
}