#pragma once

#include "BaseController.h"

namespace LCN::Controller
{
	class Camera2DController : public BaseController
	{
	public: // Signals
		Signal<Camera2DController, void()> SignlaUpdate;

	private: // Slots override
		void OnKeyPressed(        KeyPressedEvent&)         override;
		void OnMouseButtonPressed(MouseButtonPressedEvent&) override;
		void OnMouseMove(         MouseMovedEvent&)         override;
		void OnMouseScroll(       MouseScrollEvent&)        override;

	private:
		int   m_ScaleIncrement = 0;
		float m_ScaleFactor    = 1.0f;

		const float m_Ratio = 1.02f;

		Transform2Df m_TranslationStart;
		Transform2Df m_RotationStart;
	};
}