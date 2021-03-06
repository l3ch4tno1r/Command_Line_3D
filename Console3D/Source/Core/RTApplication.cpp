#include "RTApplication.h"

#include <thread>

namespace LCN::Core
{
	RTApplication::RTApplication()
	{}

	void RTApplication::Run()
	{
		Core::ConsoleInput& consoleInput = Core::ConsoleInput::Get();

		this->SignalStartup.Trigger();

		std::chrono::high_resolution_clock::time_point now, next, last;
		last = std::chrono::high_resolution_clock::now();

		while (this->IsRunning())
		{
			now = std::chrono::high_resolution_clock::now();
			float dt = (float)std::chrono::duration_cast<std::chrono::milliseconds>(now - last).count();
			next = now + m_Interval;

			this->SignalUpdate.Trigger(dt);
			this->SignalRender.Trigger();

			//Continue();

			std::this_thread::sleep_until(next);

			last = now;

			consoleInput.Continue();
		}

		this->SignalQuit.Trigger();
	}
}
