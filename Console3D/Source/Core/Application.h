#pragma once

#include <mutex>
#include <condition_variable>
#include <vector>
#include <memory>

#include <Utilities/Source/DesignPatterns/SignalSlot.h>

#include <Console3D/Source/Core/ConsoleInput.h>

int main(int argc, char** argv);

namespace LCN::Core
{
	class CWidget;

	class Application
	{
	public:
		template<class F> using SignalApplication = Signal<Application, F>;
		template<class F> using SlotApplication   = Slot<Application, F>;

		using AppPointer = std::unique_ptr<Application>;

		static Application& Get() noexcept;

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

		void RegisterWidget(CWidget& widget);

		friend int ::main(int argc, char** argv);
		friend AppPointer::deleter_type;

		// To be defined by client
		static AppPointer CreateApplication();

	private:
		inline static Application* m_App = nullptr;

		bool                    m_IsRunnning = true;
		std::mutex              m_RunMut;
		std::condition_variable m_RunCond;

		std::vector<CWidget*> m_AppWidgets;
	};
}