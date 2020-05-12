#pragma once

#include <thread>
#include <atomic>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <array>

class PaceMaker
{
private:
	//static PaceMaker s_Instance;

	std::atomic<bool>         m_Run;
	std::chrono::milliseconds m_Interval;

	std::mutex                m_NotifiedMutex;
	std::condition_variable   m_Condition;
	std::array<bool, 2>       m_Notifications;

	std::thread               m_RunThread;

	std::atomic<bool>       m_Pause;
	std::condition_variable m_PauseCondition;


	PaceMaker();
	~PaceMaker();

	PaceMaker(const PaceMaker&) = delete;
	PaceMaker& operator=(const PaceMaker&) = delete;

	void MainThread();

	void Continue();

	void NotifyAll();

public:
	static PaceMaker& Get();

	bool Heartbeat(uint32_t id);

	void Pause(bool _pause);

	void Stop();
};