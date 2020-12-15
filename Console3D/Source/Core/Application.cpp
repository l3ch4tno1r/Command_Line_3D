#include "Application.h"

#include "Devices/PaceMaker.h"
#include "Devices/EventHandler.h"
#include "Devices/Console.h"

#include <iostream>

Application& Application::Get() noexcept
{
	static Application instance;
	return instance;
}

void Application::Run()
{
	// Test
	EventHandler& eventhandler = EventHandler::Get();
	Console&      console      = Console::Get();

	PaceMaker timer;

	timer.AddObserver(eventhandler);
	timer.AddObserver(console);

	timer.Start(16ms);
	
	m_Run = true;

	Transform3Df& r0tocam = console.R0ToCam();

	Transform3Df translate;

	eventhandler.SetKeyBoardAction(Key::ESC, [this](const KeyState& state)
	{
		this->Quit();
	});

	eventhandler.Start();
	console.Start();

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
