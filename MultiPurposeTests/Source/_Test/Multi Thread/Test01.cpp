#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

using namespace std::literals::chrono_literals;

bool run    = true;
bool notify = false;

std::mutex              mut;
std::condition_variable cond;

void PaceMakerThread()
{
	for (uint32_t i = 0; i < 10; i++)
	{
		std::cout << "HeartBeat !\n";

		std::this_thread::sleep_for(1s);

		notify = true;
		cond.notify_all();
	}

	run = false;

	std::cout << "PaceMaker done !!!\n";
}

void WorkingThread()
{
	while (run)
	{
		std::unique_lock<std::mutex> lock(mut);

		std::this_thread::sleep_for(1.5s);

		while(!notify)
			cond.wait(lock);

		notify = false;

		std::cout << "Working...\n";
	}

	std::cout << "Working thread done !!!\n";
}

int main()
{
	std::thread t1(PaceMakerThread);
	std::thread t2(WorkingThread);

	t1.join();
	t2.join();

	std::cout << "Done\n";

	std::cin.get();
}