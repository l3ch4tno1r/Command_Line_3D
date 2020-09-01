#pragma once

#include <thread>
#include <atomic>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <array>

#include "Utilities/Design Patterns/Observer.h"
#include "Device.h"

class PaceMaker : public Subject<SyncDevice>
{
private:
	std::atomic<bool>         m_Run;
	std::chrono::milliseconds m_Interval;
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
	static PaceMaker& Get() noexcept;

	void Pause(bool _pause);

	void Stop();
};