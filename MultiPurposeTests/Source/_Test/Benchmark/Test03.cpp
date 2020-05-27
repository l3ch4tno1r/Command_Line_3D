#include <iostream>
#include <thread>

using namespace std::literals::chrono_literals;

#include "External/TheCherno/Instrumentor.h"

void Function()
{
	PROFILE_FUNC();

	std::cout << "Hello world !" << std::endl;

	std::this_thread::sleep_for(1s);
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
}

int main()
{
	Instrumentor::Get().BeginSession("Test");

	Test();

	Instrumentor::Get().EndSession();

	std::cin.get();
}