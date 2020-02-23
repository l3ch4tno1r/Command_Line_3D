#pragma once

#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>

class PaceMaker
{
private:
	//static PaceMaker s_Instance;

	bool                      m_Run;
	std::chrono::milliseconds m_Interval;
	std::condition_variable   m_Condition;
	std::thread               m_RunThread;

	PaceMaker();
	~PaceMaker();

	PaceMaker(const PaceMaker&) = delete;
	PaceMaker& operator=(const PaceMaker&) = delete;

	void Run();

public:
	static PaceMaker& Get();

	void Wait();

	void Wait(std::unique_lock<std::mutex>& lock);
};