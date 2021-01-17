#pragma once

#include "Application.h"

using namespace std::literals::chrono_literals;

namespace LCN
{
	class RTApplication : public Application
	{
	public:
		template<class F>
		using RTAppSignal = Signal<RTApplication, F>;

		RTApplication();
		virtual ~RTApplication() = default;

	public: // Signals
		RTAppSignal<void()>      SignalRender;
		RTAppSignal<void()>      SignalStartup;
		RTAppSignal<void(float)> SignalUpdate;
		RTAppSignal<void()>      SignalQuit;

	private:
		void Run() override;

	private:
		std::chrono::milliseconds m_Interval = 16ms;
	};
}
