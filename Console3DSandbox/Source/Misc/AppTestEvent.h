#pragma once

#include <Console3D/Source/Core/Application.h>
#include <Console3D/Source/Event/KeyBoardEvent.h>
#include <Console3D/Source/Event/MouseEvent.h>

namespace LCN
{
	class AppTestEvent : public Application
	{
	#define APPTESTEVENT_SLOT(Method, ...) SLOT(AppTestEvent, Method, __VA_ARGS__)

	public:
		template<class F>
		using SlotAppTestEvent = Slot<AppTestEvent, F>;

		AppTestEvent();

		void Run();

	public: // Slots
		APPTESTEVENT_SLOT(OnKeyPressed,  KeyPressedEvent&);
		APPTESTEVENT_SLOT(OnKeyReleased, KeyReleasedEvent&);

		APPTESTEVENT_SLOT(OnMouseMoved,          MouseMovedEvent&);
		APPTESTEVENT_SLOT(OnMouseButtonPressed,  MouseButtonPressedEvent&);
		APPTESTEVENT_SLOT(OnMouseButtonReleased, MouseButtonReleasedEvent&);
		APPTESTEVENT_SLOT(OnMouseScrolled,       MouseScrollEvent&);
	};
}

using LCNApp = LCN::AppTestEvent;