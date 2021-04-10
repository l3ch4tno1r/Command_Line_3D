#include <iostream>

#include "FPSCameraController.h"

#include "Console3D/Source/Scene/StdComponent.h"

#include <LCN_Math/Source/Utilities/Angles.h>

namespace LCN::Controller
{
	FPSCameraController::FPSCameraController() :
		m_KeyboardActions{ Core::Key::Z, Core::Key::S, Core::Key::Q, Core::Key::D }
	{}

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
		using namespace Core;

		static const size_t x = 0;
		static const size_t y = 1;
		static const size_t z = 2;

		const Transform3Df& R0ToCam = m_CameraEntt.Get<Component::Transform3DCmp>().Transform;

		auto translation = m_Translation.TranslationBlock();

		m_SpeedBoost = Core::ConsoleInput::IsKeyPressed(Core::Key::Shift) ? 10.0f : 1.0f;

		if (Core::ConsoleInput::IsKeyPressed(m_KeyboardActions[c_GoForward]))
		{
			translation(x, 0) += m_SpeedBoost * m_TranslationSpeed * R0ToCam.Rw()(x, 0) * dts;
			translation(y, 0) += m_SpeedBoost * m_TranslationSpeed * R0ToCam.Rw()(y, 0) * dts;
		}

		if (Core::ConsoleInput::IsKeyPressed(m_KeyboardActions[c_GoBackward]))
		{
			translation(x, 0) -= m_SpeedBoost * m_TranslationSpeed * R0ToCam.Rw()(x, 0) * dts;
			translation(y, 0) -= m_SpeedBoost * m_TranslationSpeed * R0ToCam.Rw()(y, 0) * dts;
		}

		if (Core::ConsoleInput::IsKeyPressed(m_KeyboardActions[c_StrafeRight]))
		{
			translation(x, 0) += m_SpeedBoost * m_TranslationSpeed * R0ToCam.Ru()(x, 0) * dts;
			translation(y, 0) += m_SpeedBoost * m_TranslationSpeed * R0ToCam.Ru()(y, 0) * dts;
		}

		if (Core::ConsoleInput::IsKeyPressed(m_KeyboardActions[c_StrafeLeft]))
		{
			translation(x, 0) -= m_SpeedBoost * m_TranslationSpeed * R0ToCam.Ru()(x, 0) * dts;
			translation(y, 0) -= m_SpeedBoost * m_TranslationSpeed * R0ToCam.Ru()(y, 0) * dts;
		}
	}

	void FPSCameraController::UpdateEntity(float dtms)
	{
		//float dts = dtms / 1000.0f;
		float dts = 16 / 1000.0f; // using dtms from app causes jittering

		this->UpdateTranslation(dts);

		if(m_Flags.RotationChanged)
			this->UpdateRotation(dts);

		Transform3Df& R0ToCam = m_CameraEntt.Get<Component::Transform3DCmp>().Transform;	

		R0ToCam = m_Translation * m_RotY * m_RotX;

		m_Flags.Total = 0;
	}

	void FPSCameraController::OnKeyPressed(KeyPressedEvent& keyEvent)
	{}

	void FPSCameraController::OnKeyReleased(KeyReleasedEvent&)
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

		this->Reset();
	}

	void FPSCameraController::OnMouseScroll(MouseScrollEvent& mouseEvent)
	{}
}