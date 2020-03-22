#ifdef TEST05

#include <iostream>

#include "Devices\PaceMaker.h"

bool run = true;

void ThreadFunc()
{
	auto id = std::this_thread::get_id();

	std::cout << id << " started !" << std::endl;

	while (run)
	{
		PaceMaker::Get().Heartbeat();

		std::cout << id << " working..." << std::endl;
	}

	std::cout << id << " done !" << std::endl;
}

int main()
{
	std::thread t1(ThreadFunc);
	std::thread t2(ThreadFunc);
	std::thread t3(ThreadFunc);

	std::cin.get();

	run = false;

	t1.join();
	t2.join();
	t3.join();

	std::cin.get();
}

#endif // TEST05
