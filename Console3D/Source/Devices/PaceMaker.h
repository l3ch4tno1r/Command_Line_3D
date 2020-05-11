#pragma once

#include <thread>
#include <atomic>
#include <chrono>
#include <mutex>
#include <condition_variable>

class PaceMaker
{
private:
	//static PaceMaker s_Instance;

	std::atomic<bool>         m_Run;
	std::chrono::milliseconds m_Interval;
	std::condition_variable   m_Condition;
	std::thread               m_RunThread;

	std::atomic<bool>       m_Pause;
	std::condition_variable m_PauseCondition;

	PaceMaker();
	~PaceMaker();

	PaceMaker(const PaceMaker&) = delete;
	PaceMaker& operator=(const PaceMaker&) = delete;

	void MainThread();

	void Continue();

public:
	static PaceMaker& Get();

	bool Heartbeat();

	void Pause(bool _pause);

	void Stop();
};