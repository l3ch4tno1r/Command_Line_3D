#pragma once

#include "Application.h"

namespace LCN
{
	class EventApplication : public Application
	{
	public:
		void Run() override;

	public: // Signals
		Signal<EventApplication, void()> SignalStartup;
		Signal<EventApplication, void()> SignalQuit;
	};
}