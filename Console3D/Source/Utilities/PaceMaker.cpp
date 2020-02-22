#include "PaceMaker.h"

using namespace std::literals::chrono_literals;

PaceMaker::PaceMaker() :
	m_Run(true),
	m_RunThread(&PaceMaker::Run, this),
	m_Interval(1s)
{}

PaceMaker::~PaceMaker()
{
	m_Run = false;
	m_RunThread.join();
}

void PaceMaker::Run()
{
	std::chrono::high_resolution_clock::time_point start, next;

	while (m_Run)
	{
		start = std::chrono::high_resolution_clock::now();
		next = start + m_Interval;

		m_Condition.notify_all();

		std::this_thread::sleep_until(next);
	}
}

//void PaceMaker::Wait()
//{
//	std::mutex mut;
//	std::unique_lock<std::mutex> lock(mut);
//	m_Condition.wait(lock);
//}

void PaceMaker::Wait(std::unique_lock<std::mutex>& lock)
{
	m_Condition.wait(lock);
}

PaceMaker& PaceMaker::Get()
{
	static PaceMaker instance;
	return instance;
}

//PaceMaker PaceMaker::s_Instance;