#include "EventApplication.h"

namespace LCN::Core
{
	void EventApplication::Run()
	{
		this->SignalStartup.Trigger();

		this->WaitQuit();

		this->SignalQuit.Trigger();
	}
}
