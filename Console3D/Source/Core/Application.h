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
		inline void KeyPressed(KeyPressedEvent& keypressedevent)    { this->SignalKeyPressed.Emmit(keypressedevent); }
		inline void KeyReleased(KeyReleasedEvent& keyreleasedevent) { this->SignalKeyReleased.Emmit(keyreleasedevent); }

		inline void MouseMove(MouseMovedEvent& mousemoveevnt)                               { this->SignalMouseMoved.Emmit(mousemoveevnt); }
		inline void MouseButtonPressed(MouseButtonPressedEvent& mousebuttonpressedevent)    { this->SignalMouseButtonPressed.Emmit(mousebuttonpressedevent); }
		inline void MouseButtonReleased(MouseButtonReleasedEvent& mousebuttonreleasedevent) { this->SignalMouseButtonReleased.Emmit(mousebuttonreleasedevent); }
		inline void MouseScrolled(MouseScrollEvent& mousescrollevent)                       { this->SignalMouseScrolled.Emmit(mousescrollevent); }

		SlotApplication<void(KeyPressedEvent&)>  SlotOnKeyPressed;
		SlotApplication<void(KeyReleasedEvent&)> SlotOnKeyReleased;

		SlotApplication<void(MouseMovedEvent&)>          SlotOnMouseMoved;
		SlotApplication<void(MouseButtonPressedEvent&)>  SlotOnMouseButtonPressed;
		SlotApplication<void(MouseButtonReleasedEvent&)> SlotOnMouseButtonReleased;
		SlotApplication<void(MouseScrollEvent&)>         SlotOnMouseScrolled;

	protected:
		Application();
	
		virtual ~Application() = default;

		void WaitQuit(); 

	private:
		virtual void Run() = 0;

		friend int ::main(int argc, char** argv);

	private:
		inline static Application* m_App = nullptr;

		bool                    m_Run = true;
		std::mutex              m_RunMut;
		std::condition_variable m_RunCond;
	};
}