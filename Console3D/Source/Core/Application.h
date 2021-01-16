#pragma once

#include <mutex>
#include <condition_variable>

#include <Utilities/Source/DesignPatterns/SignalSlot.h>
#include <Console3D/Source/Core/ConsoleInput.h>

int main(int argc, char** argv);

namespace LCN
{
	class Application
	{
	public:
		template<class F> using SignalApplication = Signal<Application, F>;
		template<class F> using SlotApplication   = Slot<Application, F>;

		template<class Derived>
		static Application& Get() noexcept
		{
			static Derived instance;
			return instance;
		}

		void Quit();

	public: // Signals
		SignalApplication<void(KeyPressedEvent&)>  SignalKeyPressed;
		SignalApplication<void(KeyReleasedEvent&)> SignalKeyReleased;

		SignalApplication<void(MouseMovedEvent&)>          SignalMouseMoved;
		SignalApplication<void(MouseButtonPressedEvent&)>  SignalMouseButtonPressed;
		SignalApplication<void(MouseButtonReleasedEvent&)> SignalMouseButtonReleased;
		SignalApplication<void(MouseScrollEvent&)>         SignalMouseScrolled;

	private: // Slots
		VIRTUAL_SLOT(Application, OnKeyPressed, KeyPressedEvent&);

	private: // Slots
		SLOT(Application, DispatchKeyPressedEvent,  KeyPressedEvent&);
		SLOT(Application, DispatchKeyReleasedEvent, KeyReleasedEvent&);

		SLOT(Application, DispatchMouseMoveEvent,           MouseMovedEvent&);
		SLOT(Application, DispatchMouseButtonPressedEvent,  MouseButtonPressedEvent&);
		SLOT(Application, DispatchMouseButtonReleasedEvent, MouseButtonReleasedEvent&);
		SLOT(Application, DispatchMouseScrolledEvent,       MouseScrollEvent&);

	protected:
		Application();

		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;
	
		virtual ~Application() = default;

		void WaitQuit();

		inline bool IsRunning() const { return m_IsRunnning; }

	private:
		virtual void Run() = 0;

		friend int ::main(int argc, char** argv);

	private:
		inline static Application* m_App = nullptr;

		bool                    m_IsRunnning = true;
		std::mutex              m_RunMut;
		std::condition_variable m_RunCond;
	};
}