#pragma once

#include <Utilities/Source/DesignPatterns/SignalSlot.h>

#include "Console3D/Source/Event/MouseEvent.h"
#include "Console3D/Source/Scene/Entity.h"

namespace LCN::Controller
{
	class Camera2DController
	{
	public:
		Camera2DController();

		void Bind(Entity camera) { m_CameraEntt = camera; }

	public: // Signals
		Signal<Camera2DController, void()> SignlaUpdate;

	private: // Slots
		SLOT(Camera2DController, OnKeyPressed,         KeyPressedEvent&);
		SLOT(Camera2DController, OnMouseButtonPressed, MouseButtonPressedEvent&);
		SLOT(Camera2DController, OnMouseMove,          MouseMovedEvent&);
		SLOT(Camera2DController, OnMouseScroll,        MouseScrollEvent&);

	private:
		Entity m_CameraEntt;

		int   m_ScaleIncrement = 0;
		float m_ScaleFactor    = 1.0f;

		const float m_Ratio = 1.02f;

		Transform2Df m_TranslationStart;
		Transform2Df m_RotationStart;
	};
}