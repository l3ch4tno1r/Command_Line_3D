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

	void FPSCameraController::UpdateEntity(float dtms)
	{
		static const size_t x = 0;
		static const size_t y = 1;
		static const size_t z = 2;

		float dts = dtms / 1000.0f;

		Transform3Df& R0ToCam = m_CameraEntt.Get<Component::Transform3DCmp>().Transform;

		m_RotX.Rv()(y, 0) =  std::cos(TORAD(m_Ax * dts));
		m_RotX.Rv()(z, 0) =  std::sin(TORAD(m_Ax * dts));
		m_RotX.Rw()(y, 0) = -std::sin(TORAD(m_Ax * dts));
		m_RotX.Rw()(z, 0) =  std::cos(TORAD(m_Ax * dts));

		m_RotY.Ru()(x, 0) =  std::cos(TORAD(m_Ay * dts));
		m_RotY.Ru()(z, 0) =  std::sin(TORAD(m_Ay * dts));
		m_RotY.Rw()(x, 0) = -std::sin(TORAD(m_Ay * dts));
		m_RotY.Rw()(z, 0) =  std::cos(TORAD(m_Ay * dts));

		R0ToCam = m_Translation * m_RotY * m_RotX;
	}

	void FPSCameraController::OnKeyPressed(KeyPressedEvent& keyEvent)
	{}

	void FPSCameraController::OnMouseButtonPressed(MouseButtonPressedEvent& mouseEvent)
	{}

	void FPSCameraController::OnMouseMove(MouseMovedEvent& mouseEvent)
	{
		Vector2Di mousePos = Vector2Di({ mouseEvent.X(), mouseEvent.Y() }) - m_Center;

		m_Ay += mousePos.x() * m_AngleSpeed;
		m_Ax += mousePos.y() * m_AngleSpeed;

		using namespace Core;

		//m_Ax = std::clamp(m_Ax, -80.0f, 80.0f);
		Console::Get().Message(Console::MessageSlots::_1) << '(' << mouseEvent.X() << ", " << mouseEvent.Y() << ')';

		this->Reset();
	}

	void FPSCameraController::OnMouseScroll(MouseScrollEvent& mouseEvent)
	{}
}