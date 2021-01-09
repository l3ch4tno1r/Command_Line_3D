#include <iostream>

#include "AppTest.h"
#include "Core/Devices/Console.h"
#include "RessourceManagement/OBJReader.h"
#include "LibraryMapping/Math.h"

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

	//Model3D model = OBJReader().ReadFile<Model3D>("Ressource/cube.obj", false);

	console.ConstructConsole(150, 100, 8, 8);

	console.Clear();

	Vector2f v1 = {  50, 20 };
	Vector2f v2 = { 120, 30 };
	Vector2f v3 = {  75, 80 };

	console.FillTriangleOLC(
		v1.x(), v1.y(),
		v2.x(), v2.y(),
		v3.x(), v3.y(),
		[](int x, int y)
		{
			CHAR_INFO result;

			result.Char.UnicodeChar = 0;

			switch ((x + y) % 3)
			{
			case 0:
				result.Attributes = COLOUR::BG_RED;
				break;
			case 1:
				result.Attributes = COLOUR::BG_GREEN;
				break;
			case 2:
				result.Attributes = COLOUR::BG_BLUE;
				break;
			default:
				break;
			}

			return result;
		});

	console.Render();

	std::cin.get();
}

void AppTest::WaitQuit()
{
	std::unique_lock<std::mutex> lock(m_RunMut);

	while (m_Run)
		m_RunCond.wait(lock);
}
