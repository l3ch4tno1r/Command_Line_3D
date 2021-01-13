#pragma once

#include <Console3D/Source/Core/Application.h>
#include <Console3D/Source/Event/KeyBoardEvent.h>

namespace LCN
{
	class AppTestEvent : public Application<AppTestEvent>
	{
	public:
		AppTestEvent();

		void Run();

	public: // Slots
		void OnKeyPressed(KeyPressedEvent& keypressedevent);
		void OnKeyReleased(KeyReleasedEvent& keyreleasedevent);

		Slot<AppTestEvent, void(KeyPressedEvent&)>  SlotOnKeyPressed;
		Slot<AppTestEvent, void(KeyReleasedEvent&)> SlotOnKeyReleased;
	};
}

using LCNApp = LCN::AppTestEvent;