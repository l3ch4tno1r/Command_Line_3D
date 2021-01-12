#include "AppTestEvent.h"

#include <Console3D/Source/Core/EventHandler.h>

void LCN::AppTestEvent::Run()
{
	auto& eventhandler = EventHandler::Get();

	eventhandler.Start();

	this->WaitQuit();
}
