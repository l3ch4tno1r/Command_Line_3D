#pragma once

#include <Console3D/Source/Core/Application.h>
#include <Console3D/Source/Event/KeyBoardEvent.h>
#include <Console3D/Source/Event/MouseEvent.h>

namespace LCN
{
	class AppTestEvent : public Application<AppTestEvent>
	{
	public:
		template<class F>
		using SlotAppTestEvent = Slot<AppTestEvent, F>;

		AppTestEvent();

		void Run();

	public: // Slots
		void OnKeyPressed(KeyPressedEvent& keypressedevent);
		void OnKeyReleased(KeyReleasedEvent& keyreleasedevent);

		void OnMouseMoved(MouseMovedEvent& mousemoveevent);
		void OnMouseButtonPressed(MouseButtonPressedEvent& mousebuttonpressedevent);
		void OnMouseButtonReleased(MouseButtonReleasedEvent& mousebuttonreleasedevent);

		SlotAppTestEvent<void(KeyPressedEvent&)>  SlotOnKeyPressed;
		SlotAppTestEvent<void(KeyReleasedEvent&)> SlotOnKeyReleased;

		SlotAppTestEvent<void(MouseMovedEvent&)>          SlotMouseMove;
		SlotAppTestEvent<void(MouseButtonPressedEvent&)>  SlotMouseButtonPressed;
		SlotAppTestEvent<void(MouseButtonReleasedEvent&)> SlotMouseButtonReleased;
	};
}

using LCNApp = LCN::AppTestEvent;