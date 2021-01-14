#include <iostream>

#include "AppTestEvent.h"

namespace LCN
{
	AppTestEvent::AppTestEvent() :
		SLOT_INIT(SlotOnKeyPressed,          AppTestEvent::OnKeyPressed),
		SLOT_INIT(SlotOnKeyReleased,         AppTestEvent::OnKeyReleased),
		SLOT_INIT(SlotOnMouseMoved,          AppTestEvent::OnMouseMoved),
		SLOT_INIT(SlotOnMouseButtonPressed,  AppTestEvent::OnMouseButtonPressed),
		SLOT_INIT(SlotOnMouseButtonReleased, AppTestEvent::OnMouseButtonReleased)
	{		
		Connect(this->SignalKeyPressed,          this->SlotOnKeyPressed);
		Connect(this->SignalKeyReleased,         this->SlotOnKeyReleased);
		Connect(this->SignalMouseMoved,          this->SlotOnMouseMoved);
		Connect(this->SignalMouseButtonPressed,  this->SlotOnMouseButtonPressed);
		Connect(this->SignalMouseButtonReleased, this->SlotOnMouseButtonReleased);
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