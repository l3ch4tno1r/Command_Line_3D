#include "RTApplication.h"

#include <thread>

namespace LCN
{
	RTApplication::RTApplication()
	{}

	void RTApplication::Run()
	{
		auto next = std::chrono::high_resolution_clock::now();

		while (this->IsRunning())
		{
			next += m_Interval;

			this->Heartbeat.Emmit();

			//Continue();

			std::this_thread::sleep_until(next);
		}
	}
}
