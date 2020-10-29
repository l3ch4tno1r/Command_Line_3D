#include "Application.h"

#include "Devices/PaceMaker.h"
#include "Devices/EventHandler.h"

#include <iostream>

Application& Application::Get() noexcept
{
	static Application instance;
	return instance;
}

void Application::Run()
{
	EventHandler& eventhandler = EventHandler::Get();

	PaceMaker timer;

	timer.AddObserver(eventhandler);
	timer.Start(16ms);
	
	m_Run = true;

	eventhandler.SetKeyBoardAction(Key::ESC, [this](const KeyState& state)
	{
		if (state.KeyPressed)
			this->Quit();
	});

	eventhandler.SetKeyBoardAction(Key::A, [](const KeyState& state)
	{
		if(state.KeyPressed)
			std::cout << "Pressed" << std::endl;

		//if (state.KeyReleased)
		//	std::cout << "Released" << std::endl;
	});

	eventhandler.SetMouseAction(MouseButton::LEFT, [](const KeyState& state, int x, int y)
	{
		if(state.KeyPressed)
			std::cout << "Left button pressed at (" << x << ", " << y << ')' << std::endl;

		//if (state.KeyReleased)
		//	std::cout << "Left button released at (" << x << ", " << y << ')' << std::endl;
	});

	eventhandler.Start();

	this->WaitQuit();
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
