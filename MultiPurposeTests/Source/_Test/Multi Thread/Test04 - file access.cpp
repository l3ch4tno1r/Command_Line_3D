#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <fstream>

using namespace std::literals::chrono_literals;

bool notified = false;
std::mutex mut;
std::condition_variable cond;

void Wait()
{
	std::unique_lock<std::mutex> lock(mut);

	while (!notified)
		cond.wait(lock);
}

void TestThread()
{
	std::ofstream out("shared_file.txt", std::ios::out | std::ios::app);

	if (out)
		out << "Hello world ! - " << std::this_thread::get_id() << std::endl;

	Wait();
}

int main()
{
	std::thread t1(TestThread);
	std::thread t2(TestThread);

	std::cout << "Done" << std::endl;

	std::cin.get();

	notified = true;

	cond.notify_all();

	t1.join();
	t2.join();
}