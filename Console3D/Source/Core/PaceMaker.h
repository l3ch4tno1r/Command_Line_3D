#pragma once

#include <thread>
#include <atomic>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <array>

#include "Utilities/Source/DesignPatterns/Observer.h"
#include "Device.h"

using namespace std::literals::chrono_literals;

namespace LCN::Core
{
	class PaceMaker : public Subject<Device>
	{
	public:
		PaceMaker();

		~PaceMaker();

		void Start(std::chrono::milliseconds interval);

		void Pause(bool _pause);

		void Stop();

	private:
		std::atomic<bool>         m_Run;
		std::chrono::milliseconds m_Interval;
		std::thread               m_RunThread;

		std::atomic<bool>       m_Pause;
		std::condition_variable m_PauseCondition;

		PaceMaker(const PaceMaker&) = delete;
		PaceMaker& operator=(const PaceMaker&) = delete;

		void MainThread();

		void Continue();
	};
}