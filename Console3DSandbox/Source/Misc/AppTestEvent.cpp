#include <iostream>

#include "AppTestEvent.h"

namespace LCN
{
	AppTestEvent::AppTestEvent() :
		SLOT_INIT(SlotOnKeyPressed,          AppTestEvent::OnKeyPressed),
		SLOT_INIT(SlotOnKeyReleased,         AppTestEvent::OnKeyReleased),
		SLOT_INIT(SlotOnMouseMoved,          AppTestEvent::OnMouseMoved),
		SLOT_INIT(SlotOnMouseButtonPressed,  AppTestEvent::OnMouseButtonPressed),
		SLOT_INIT(SlotOnMouseButtonReleased, AppTestEvent::OnMouseButtonReleased),
		SLOT_INIT(SlotOnMouseScrolled,       AppTestEvent::OnMouseScrolled)
	{		
		Connect(this->SignalKeyPressed,          this->SlotOnKeyPressed);
		Connect(this->SignalKeyReleased,         this->SlotOnKeyReleased);
		Connect(this->SignalMouseMoved,          this->SlotOnMouseMoved);
		Connect(this->SignalMouseButtonPressed,  this->SlotOnMouseButtonPressed);
		Connect(this->SignalMouseButtonReleased, this->SlotOnMouseButtonReleased);
		Connect(this->SignalMouseScrolled,       this->SlotOnMouseScrolled);
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
		std::cout << "Key #" << keyreleasedevent.KeyCode() << " released." << std::endl;
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

	void AppTestEvent::OnMouseScrolled(MouseScrollEvent& mousescrollevent)
	{
		switch (mousescrollevent.ScrollDirection())
		{
		case -1:
			std::cout << "Mouse scrolled down." << std::endl;
			break;
		case 1:
			std::cout << "Mouse scrolled up." << std::endl;
			break;
		default:
			break;
		}
	}
}