#include <iostream>
#include <thread>
#include <atomic>

std::atomic<int> global = 0;

void Task()
{
	for (int i = 0; i < 1000000; ++i)
		++global;
}

int main()
{
	std::thread threads[] = {
		std::thread(Task),
		std::thread(Task),
		std::thread(Task)
	};

	for (auto& t : threads)
		t.join();

	std::cout << global << std::endl;

	std::cin.get();
}