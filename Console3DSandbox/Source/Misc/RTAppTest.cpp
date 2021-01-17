#include "RTAppTest.h"

#include <iostream>

namespace LCN
{
	RTAppTest::RTAppTest() :
		SLOT_INIT(SlotOnStartup, RTAppTest::OnStartup),
		SLOT_INIT(SlotOnUpdate,  RTAppTest::OnUpdate),
		SLOT_INIT(SlotOnRender,  RTAppTest::OnRender),
		SLOT_INIT(SlotOnQuit,    RTAppTest::OnQuit)
	{
		Connect(this->SignalStartup, this->SlotOnStartup);
		Connect(this->SignalUpdate,  this->SlotOnUpdate);
		Connect(this->SignalRender,  this->SlotOnRender);
		Connect(this->SignalQuit,    this->SlotOnQuit);
	}

	void RTAppTest::OnStartup()
	{
		std::cout << "Startup." << std::endl;
	}

	void RTAppTest::OnUpdate(float delta)
	{
		std::cout << "Updating with delta = " << delta << std::endl;
	}

	void RTAppTest::OnRender()
	{
		std::cout << "Rendering..." << std::endl;

		std::this_thread::sleep_for(1s);
	}

	void RTAppTest::OnQuit()
	{
		std::cout << "Quit" << std::endl;
	}
}
