#include "Application.h"

namespace LCN
{
	Application::Application() :
		SLOT_INIT(SlotOnKeyPressed,           Application::KeyPressed),
		SLOT_INIT(SlotOnKeyReleased,          Application::KeyReleased),
		SLOT_INIT(SlotOnMouseMoved,           Application::MouseMove),
		SLOT_INIT(SlotOnMouseButtonPressed,   Application::MouseButtonPressed),
		SLOT_INIT(SlotOnMouseButtonReleased,  Application::MouseButtonReleased)
	{
		if (m_App)
			throw std::exception("Application is already running.");

		m_App = this;

		ConsoleInput& consoleinput = ConsoleInput::Get();

		Connect(consoleinput.SignalKeyPressed,            this->SlotOnKeyPressed);
		Connect(consoleinput.SignalKeyReleased,           this->SlotOnKeyReleased);
		Connect(consoleinput.SignalMouseMove,             this->SlotOnMouseMoved);
		Connect(consoleinput.SignalMouseButtonPressed,    this->SlotOnMouseButtonPressed);
		Connect(consoleinput.SignalMouseButtonReleased,   this->SlotOnMouseButtonReleased);

		consoleinput.Start();
	}

	void Application::Quit()
	{
		std::lock_guard<std::mutex> lock(m_RunMut);

		m_Run = false;

		m_RunCond.notify_one();
	}

	void Application::WaitQuit()
	{
		std::unique_lock<std::mutex> lock(m_RunMut);

		while (m_Run)
			m_RunCond.wait(lock);
	}
}