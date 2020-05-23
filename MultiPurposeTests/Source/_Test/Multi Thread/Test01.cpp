#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <chrono>

#include "Source/Utils/TraceLog.h"

using namespace std::literals::chrono_literals;
using namespace LCNUtilities;

std::atomic<bool> run    = true;
std::atomic<bool> notify = false;

std::mutex              mut;
std::condition_variable cond;

void PaceMakerThread()
{
	for (uint32_t i = 0; i < 10; i++)
	{
		Log() << "HeartBeat !";

		std::this_thread::sleep_for(1s);

		notify = true;
		cond.notify_all();
	}

	run = false;

	Log() << "PaceMaker done !!!";
}

void WorkingThread()
{
	std::thread::id id = std::this_thread::get_id();

	while (run)
	{
		std::this_thread::sleep_for(1.5s);

		std::unique_lock<std::mutex> lock(mut);

		while(!notify)
			cond.wait(lock);

		notify = false;

		Log() << id << " working...";
	}

	Log() << "Working thread done !!!";
}

int main()
{
	std::thread threads[] = {
		std::thread(PaceMakerThread),
		std::thread(WorkingThread),
		std::thread(WorkingThread)
	};

	for (auto& t : threads)
		t.join();

	Log() << "Done";

	std::cin.get();
}