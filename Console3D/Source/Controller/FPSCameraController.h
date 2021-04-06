#pragma once

#include "BaseController.h"

#include <LCN_Math/Source/Geometry/Geometry.h>

namespace LCN::Controller
{
	class FPSCameraController : public BaseController
	{
	public:
		void Init();
		void Reset();
		void UpdateEntity(float dtms);

	private: // Slots override
		void OnKeyPressed(KeyPressedEvent&) override;
		void OnMouseButtonPressed(MouseButtonPressedEvent&) override;
		void OnMouseMove(MouseMovedEvent&) override;
		void OnMouseScroll(MouseScrollEvent&) override;

	private:
		void UpdateRotation(float dts);
		void UpdateTranslation(float dts);

	private:
		Vector2Di m_Center;

		float m_Ax = 0.0f, m_Ay = 0.0f;
		float m_AngleSpeed = 5.0f;

		Transform3Df m_Translation;
		Transform3Df m_RotX;
		Transform3Df m_RotY;

		union
		{
			struct
			{
				bool RotationChanged    : 1;
				bool TranslationChanged : 1;
			};
			char Total = 0;
		} m_Flags;
	};
}