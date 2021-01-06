#include <iostream>

#include "AppTest.h"
#include "Core/Devices/Console.h"

AppTest& AppTest::Get() noexcept
{
	static AppTest instance;
	return instance;
}

void AppTest::Quit()
{
	std::lock_guard<std::mutex> lock(m_RunMut);

	m_Run = false;

	m_RunCond.notify_one();
}

void AppTest::Run()
{
	Console& console = Console::Get();

	console.ConstructConsole(150, 100, 8, 8);

	console.Clear();

	int x1 =  50, y1 = 20;
	int x2 = 120, y2 = 30;
	int x3 =  75, y3 = 80;

	console.FillTriangleOLC(
		x1, y1,
		x2, y2,
		x3, y3,
		[](int x, int y)
		{
			CHAR_INFO result;

			result.Char.UnicodeChar = 0;
			result.Attributes = (x + y) % 2 == 0 ? COLOUR::BG_GREEN : COLOUR::BG_RED;

			return result;
		}
	);

	console.Render();

	std::cin.get();
}

void AppTest::WaitQuit()
{
	std::unique_lock<std::mutex> lock(m_RunMut);

	while (m_Run)
		m_RunCond.wait(lock);
}
