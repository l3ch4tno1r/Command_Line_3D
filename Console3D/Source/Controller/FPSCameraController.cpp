#include <iostream>

#include "FPSCameraController.h"

#include "Console3D/Source/Core/ConsoleInput.h"
#include "Console3D/Source/Core/Console.h"
#include "Console3D/Source/Scene/StdComponent.h"

#include <LCN_Math/Source/Utilities/Angles.h>

namespace LCN::Controller
{
	void FPSCameraController::Init()
	{
		m_Translation = m_CameraEntt.Get<Component::Transform3DCmp>().Transform;

		this->Reset();
	}

	void FPSCameraController::Reset()
	{
		auto& consoleInput = Core::ConsoleInput::Get();

		auto [Cx, Cy] = consoleInput.GetWindowCenter();

		m_Center = { Cx, Cy };

		consoleInput.SetCursorPosition(Cx, Cy);
	}

	void FPSCameraController::UpdateRotation(float dts)
	{
		static const size_t x = 0;
		static const size_t y = 1;
		static const size_t z = 2;

		float Ax = std::clamp(m_Ax * dts, -80.0f, 80.0f);
		float Ay = m_Ay * dts;

		m_RotX.Rv()(y, 0) =  std::cos(TORAD(Ax));
		m_RotX.Rv()(z, 0) =  std::sin(TORAD(Ax));
		m_RotX.Rw()(y, 0) = -std::sin(TORAD(Ax));
		m_RotX.Rw()(z, 0) =  std::cos(TORAD(Ax));

		m_RotY.Ru()(x, 0) =  std::cos(TORAD(Ay));
		m_RotY.Ru()(z, 0) =  std::sin(TORAD(Ay));
		m_RotY.Rw()(x, 0) = -std::sin(TORAD(Ay));
		m_RotY.Rw()(z, 0) =  std::cos(TORAD(Ay));
	}

	void FPSCameraController::UpdateTranslation(float dts)
	{
	}

	void FPSCameraController::UpdateEntity(float dtms)
	{
		if (m_Flags.Total == 0)
			return;

		//float dts = dtms / 1000.0f;
		float dts = 16 / 1000.0f; // using dtms from app causes jittering

		if(m_Flags.RotationChanged)
			this->UpdateRotation(dts);

		if(m_Flags.TranslationChanged)
			this->UpdateTranslation(dts);

		Transform3Df& R0ToCam = m_CameraEntt.Get<Component::Transform3DCmp>().Transform;	

		R0ToCam = m_Translation * m_RotY * m_RotX;

		m_Flags.Total = 0;
	}

	void FPSCameraController::OnKeyPressed(KeyPressedEvent& keyEvent)
	{}

	void FPSCameraController::OnMouseButtonPressed(MouseButtonPressedEvent& mouseEvent)
	{}

	void FPSCameraController::OnMouseMove(MouseMovedEvent& mouseEvent)
	{
		if (mouseEvent.X() == m_Center.x() && mouseEvent.Y() == m_Center.y())
			return;

		m_Flags.RotationChanged = true;

		Vector2Di mousePos = Vector2Di({ mouseEvent.X(), mouseEvent.Y() }) - m_Center;

		m_Ay -= mousePos.x() * m_AngleSpeed;
		m_Ax += mousePos.y() * m_AngleSpeed;
		
		//m_Ax = std::clamp(m_Ax, -80.0f, 80.0f);
		Core::Console::Get().Message(Core::Console::MessageSlots::_1) << m_Ax;

		this->Reset();
	}

	void FPSCameraController::OnMouseScroll(MouseScrollEvent& mouseEvent)
	{}
}