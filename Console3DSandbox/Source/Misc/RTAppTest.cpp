#include "RTAppTest.h"

#include <Console3D/Source/Core/Console.h>

#include <Utilities/Source/Instrumentor.h>

#include <iostream>
#include <sstream>

namespace LCN
{
	RTAppTest::RTAppTest()
	{
		Connect(this->SignalStartup, this->SlotOnStartup);
		Connect(this->SignalUpdate,  this->SlotOnUpdate);
		Connect(this->SignalRender,  this->SlotOnRender);
		Connect(this->SignalQuit,    this->SlotOnQuit);
	}

	void RTAppTest::OnStartup()
	{
		auto& console = Core::Console::Get();

		console.ConstructConsole(150, 100, 8, 8);

		Instrumentor::Get().BeginSession("RTAppTest");
	}

	void RTAppTest::OnUpdate(float delta)
	{
		PROFILE_FUNC();

		auto& console = Core::Console::Get();
		
		std::stringstream sstr;

		sstr << "Update delta " << delta << "ms";

		console.Clear();

		console.DisplayMessage(sstr.str(), Core::Console::MessageSlots::_5);

		console.Render();
	}

	void RTAppTest::OnRender()
	{
		//std::cout << "Rendering..." << std::endl;

		//std::this_thread::sleep_for(16ms);
	}

	void RTAppTest::OnQuit()
	{
		Instrumentor::Get().EndSession();
	}

	CREATE_APP(RTAppTest);
}
