#include "PaceMaker.h"

PaceMaker::PaceMaker() :
	m_Run(true),
	m_Pause(false)
{}

PaceMaker::~PaceMaker()
{
	m_Run = false;
	m_RunThread.join();
}

void PaceMaker::Start(std::chrono::milliseconds interval)
{
	m_Interval = interval;

	m_RunThread = std::thread(&PaceMaker::MainThread, this);
}

void PaceMaker::MainThread()
{
	std::chrono::high_resolution_clock::time_point start, next;

	while (m_Run)
	{
		start = std::chrono::high_resolution_clock::now();
		next  = start + m_Interval;

		Continue();

		Notify(true);

		std::this_thread::sleep_until(next);
	}
}

void PaceMaker::Continue()
{
	static std::mutex mut;
	std::unique_lock<std::mutex> lock(mut);

	while (m_Pause)
		m_PauseCondition.wait(lock);
}

void PaceMaker::Pause(bool _pause)
{
	m_Pause = _pause;

	m_PauseCondition.notify_one();
}

void PaceMaker::Stop()
{
	m_Run = false;
}