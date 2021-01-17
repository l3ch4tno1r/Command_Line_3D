#include "RTApplication.h"

#include <thread>

namespace LCN
{
	RTApplication::RTApplication()
	{}

	void RTApplication::Run()
	{
		this->SignalStartup.Emmit();

		std::chrono::high_resolution_clock::time_point now, next, last;
		last = std::chrono::high_resolution_clock::now();

		while (this->IsRunning())
		{
			now = std::chrono::high_resolution_clock::now();
			float delta = (float)std::chrono::duration_cast<std::chrono::milliseconds>(now - last).count() / 1000.0f;
			next = now + m_Interval;

			this->SignalUpdate.Emmit(delta);
			this->SignalRender.Emmit();

			//Continue();

			std::this_thread::sleep_until(next);

			last = now;
		}

		this->SignalQuit.Emmit();
	}
}
