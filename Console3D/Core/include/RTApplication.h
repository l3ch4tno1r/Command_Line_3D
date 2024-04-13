#pragma once

#include "Application.h"

using namespace std::literals::chrono_literals;

namespace LCN::Core
{
	class RTApplication : public Application
	{
	public:
		template<class F>
		using RTAppSignal = Signal<RTApplication, F>;

		RTApplication();
		virtual ~RTApplication() = default;

	protected: // Signals
		RTAppSignal<void()>      SignalRender;
		RTAppSignal<void()>      SignalStartup;
		RTAppSignal<void(float)> SignalUpdate;
		RTAppSignal<void()>      SignalQuit;

	private:
		void Run() override;

	private:
		const std::chrono::milliseconds m_Interval = 16ms;
	};
}
