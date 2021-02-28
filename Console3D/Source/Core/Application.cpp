#include "Application.h"

namespace LCN
{
	Application::Application() :
		SLOT_INIT(SlotDispatchKeyPressedEvent,          Application::DispatchKeyPressedEvent),
		SLOT_INIT(SlotDispatchKeyReleasedEvent,         Application::DispatchKeyReleasedEvent),
		SLOT_INIT(SlotDispatchMouseMoveEvent,           Application::DispatchMouseMoveEvent),
		SLOT_INIT(SlotDispatchMouseButtonPressedEvent,  Application::DispatchMouseButtonPressedEvent),
		SLOT_INIT(SlotDispatchMouseButtonReleasedEvent, Application::DispatchMouseButtonReleasedEvent),
		SLOT_INIT(SlotDispatchMouseScrolledEvent,       Application::DispatchMouseScrolledEvent),
		SLOT_INIT(SlotOnKeyPressed, Application::OnKeyPressed)
	{
		if (m_App)
			throw std::exception("Application is already running.");

		m_App = this;

		ConsoleInput& consoleinput = ConsoleInput::Get();

		Connect(consoleinput.SignalKeyPressed,          this->SlotDispatchKeyPressedEvent);
		Connect(consoleinput.SignalKeyReleased,         this->SlotDispatchKeyReleasedEvent);
		Connect(consoleinput.SignalMouseMove,           this->SlotDispatchMouseMoveEvent);
		Connect(consoleinput.SignalMouseButtonPressed,  this->SlotDispatchMouseButtonPressedEvent);
		Connect(consoleinput.SignalMouseButtonReleased, this->SlotDispatchMouseButtonReleasedEvent);
		Connect(consoleinput.SignalMouseScroll,         this->SlotDispatchMouseScrolledEvent);

		Connect(consoleinput.SignalKeyPressed, this->SlotOnKeyPressed);

		consoleinput.Start();
	}

	Application& Application::Get() noexcept
	{
		static AppPointer instance = CreateApplication();
		return *instance;
	}

	void Application::Quit()
	{
		std::lock_guard<std::mutex> lock(m_RunMut);

		if (!m_IsRunnning)
			return;

		m_IsRunnning = false;

		m_RunCond.notify_one();
	}

	void Application::DispatchKeyPressedEvent(KeyPressedEvent& keypressedevent)    { this->SignalKeyPressed.Trigger(keypressedevent); }
	void Application::DispatchKeyReleasedEvent(KeyReleasedEvent& keyreleasedevent) { this->SignalKeyReleased.Trigger(keyreleasedevent); }

	void Application::DispatchMouseMoveEvent(MouseMovedEvent& mousemovedevent)                             { this->SignalMouseMoved.Trigger(mousemovedevent); }
	void Application::DispatchMouseButtonPressedEvent(MouseButtonPressedEvent& mousebuttonpressedevent)    { this->SignalMouseButtonPressed.Trigger(mousebuttonpressedevent); }
	void Application::DispatchMouseButtonReleasedEvent(MouseButtonReleasedEvent& mousebuttonreleasedevent) { this->SignalMouseButtonReleased.Trigger(mousebuttonreleasedevent); }
	void Application::DispatchMouseScrolledEvent(MouseScrollEvent& mousescrollevent)                       { this->SignalMouseScrolled.Trigger(mousescrollevent); }

	void Application::WaitQuit()
	{
		std::unique_lock<std::mutex> lock(m_RunMut);

		while (m_IsRunnning)
			m_RunCond.wait(lock);
	}

	void Application::RegisterWidget(CWidget& widget)
	{
		m_AppWidgets.push_back(&widget);
	}

	void Application::OnKeyPressed(KeyPressedEvent& keypressedevent)
	{
		switch (keypressedevent.KeyCode())
		{
		case Key::Esc:
			this->Quit();
			break;
		default:
			break;
		}
	}
}