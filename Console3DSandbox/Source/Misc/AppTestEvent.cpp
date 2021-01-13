#include <iostream>

#include "AppTestEvent.h"

#include <Console3D/Source/Core/EventHandler.h>

namespace LCN
{
	AppTestEvent::AppTestEvent() :
		SlotOnKeyPressed( SLOT_INIT(AppTestEvent::OnKeyPressed)),
		SlotOnKeyReleased(SLOT_INIT(AppTestEvent::OnKeyReleased))
	{}

	void AppTestEvent::Run()
	{
		Bind(this->SignalKeyPressed,  this->SlotOnKeyPressed);
		Bind(this->SignalKeyReleased, this->SlotOnKeyReleased);

		this->WaitQuit();
	}

	void AppTestEvent::OnKeyPressed(KeyPressedEvent& keypressedevent)
	{
		switch (keypressedevent.KeyCode())
		{
		case Key::ESC:
			this->Quit();
			break;
		default:
			std::cout << "Key #" << keypressedevent.KeyCode() << " pressed." << std::endl;
			break;
		}
	}

	void AppTestEvent::OnKeyReleased(KeyReleasedEvent& keyreleasedevent)
	{
		switch (keyreleasedevent.KeyCode())
		{
		default:
			std::cout << "Key #" << keyreleasedevent.KeyCode() << " released." << std::endl;
			break;
		}
	}
}

