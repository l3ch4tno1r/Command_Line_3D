#include "Application.h"

#include "Devices/PaceMaker.h"
#include "Devices/EventHandler.h"

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

	eventhandler.AddKeyBoardAction(VK_ESCAPE, [this](const KeyState& state)
	{
		if (state.KeyPressed)
			this->Quit();
	});

	eventhandler.Start();

	this->Wait();
}

void Application::Quit()
{
	std::lock_guard<std::mutex> lock(m_RunMut);

	m_Run = false;

	m_RunCond.notify_one();
}

void Application::Wait()
{
	std::unique_lock<std::mutex> lock(m_RunMut);

	while (m_Run)
		m_RunCond.wait(lock);
}
