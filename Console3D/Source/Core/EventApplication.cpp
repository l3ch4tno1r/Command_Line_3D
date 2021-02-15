#include "EventApplication.h"

namespace LCN
{
	void EventApplication::Run()
	{
		this->SignalStartup.Trigger();

		this->WaitQuit();

		this->SignalQuit.Trigger();
	}
}
