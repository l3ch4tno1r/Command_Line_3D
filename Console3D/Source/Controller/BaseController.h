#pragma once

#include <Utilities/Source/DesignPatterns/SignalSlot.h>

#include "Console3D/Source/Event/MouseEvent.h"
#include "Console3D/Source/Event/KeyBoardEvent.h"

#include "Console3D/Source/Scene/Entity.h"

namespace LCN::Controller
{
	class BaseController
	{
	public:
		void Bind(Entity camera) { m_CameraEntt = camera; }

	protected:
		Entity m_CameraEntt;

		BaseController();

	private: // Slots
		ABSTRACT_SLOT(BaseController, OnKeyPressed,         KeyPressedEvent&);
		ABSTRACT_SLOT(BaseController, OnKeyReleased,        KeyReleasedEvent&);
		ABSTRACT_SLOT(BaseController, OnMouseButtonPressed, MouseButtonPressedEvent&);
		ABSTRACT_SLOT(BaseController, OnMouseMove,          MouseMovedEvent&);
		ABSTRACT_SLOT(BaseController, OnMouseScroll,        MouseScrollEvent&);
	};
}