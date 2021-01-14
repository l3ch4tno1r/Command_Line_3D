#include <iostream>

#include "AppTestEvent.h"

namespace LCN
{
	AppTestEvent::AppTestEvent() :
		SLOT_INIT(SlotOnKeyPressed,        AppTestEvent::OnKeyPressed),
		SLOT_INIT(SlotOnKeyReleased,       AppTestEvent::OnKeyReleased),
		SLOT_INIT(SlotMouseMove,           AppTestEvent::OnMouseMoved),
		SLOT_INIT(SlotMouseButtonPressed,  AppTestEvent::OnMouseButtonPressed),
		SLOT_INIT(SlotMouseButtonReleased, AppTestEvent::OnMouseButtonReleased)
	{		
		Connect(this->SignalKeyPressed,          this->SlotOnKeyPressed);
		Connect(this->SignalKeyReleased,         this->SlotOnKeyReleased);
		Connect(this->SignalMouseMoved,          this->SlotMouseMove);
		Connect(this->SignalMouseButtonPressed,  this->SlotMouseButtonPressed);
		Connect(this->SignalMouseButtonReleased, this->SlotMouseButtonReleased);
	}

	void AppTestEvent::Run()
	{
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

	void AppTestEvent::OnMouseMoved(MouseMovedEvent& mousemoveevent)
	{}

	void AppTestEvent::OnMouseButtonPressed(MouseButtonPressedEvent& mousebuttonpressedevent)
	{
		std::cout << "Mouse #" << mousebuttonpressedevent.ButtonCode() << " : (" << mousebuttonpressedevent.X() << ", " << mousebuttonpressedevent.Y() << ')' << std::endl;
	}

	void AppTestEvent::OnMouseButtonReleased(MouseButtonReleasedEvent& mousebuttonreleasedevent)
	{
		std::cout << "Mouse #" << mousebuttonreleasedevent.ButtonCode() << " : (" << mousebuttonreleasedevent.X() << ", " << mousebuttonreleasedevent.Y() << ')' << std::endl;
	}
}