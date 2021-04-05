#include "Console3D/Source/Core/ConsoleInput.h"
#include "Console3D/Source/Scene/StdComponent.h"

#include "Camera2DController.h"

#include <cmath>

namespace LCN::Controller
{
	void Camera2DController::OnKeyPressed(KeyPressedEvent& keyevent)
	{
		switch (static_cast<Core::Key>(keyevent.KeyCode()))
		{
		case Core::Key::R:
			m_TranslationStart = Transform2Df::Identity();
			m_RotationStart    = Transform2Df::Identity();

			m_CameraEntt.Get<Component::Transform2DCmp>().Transform = Transform2Df::Identity();

			m_ScaleFactor    = 1.0f;
			m_ScaleIncrement = 0;

			this->SignlaUpdate.Trigger();

			break;
		default:
			break;
		}
	}

	void Camera2DController::OnMouseScroll(MouseScrollEvent& mouseevent)
	{
		m_ScaleIncrement = -(int)mouseevent.ScrollDirection();

		m_ScaleFactor = std::pow(m_Ratio, m_ScaleIncrement);

		auto& R0ToCam = m_CameraEntt.Get<Component::Transform2DCmp>().Transform;

		Transform2Df temp = m_ScaleFactor * R0ToCam;
		R0ToCam = temp;

		// TODO : Use a scaling matrix ?
		R0ToCam(2, 2) = 1.0f;

		this->SignlaUpdate.Trigger();
	}

	void Camera2DController::OnMouseButtonPressed(MouseButtonPressedEvent& mouseevent)
	{
		Transform2Df& PixToCam = m_CameraEntt.Get<Component::CameraCmp>().PixToCam;
		HVector2Df pos = PixToCam.QuickInverse()  * HVector2Df({ (float)mouseevent.X(), (float)mouseevent.Y(), 1.0f });

		m_TranslationStart.TranslationBlock() = pos.Vector();

		Vector2Df maindir = pos.Vector();
		
		maindir = maindir / maindir.Norm();
		
		m_RotationStart.RotationBlock() = {
			maindir.x(), -maindir.y(),
			maindir.y(),  maindir.x()
		};
	}

	void Camera2DController::OnMouseMove(MouseMovedEvent& mouseevent)
	{
		if (Core::ConsoleInput::IsMouseBtnPressed(Core::MouseButton::LEFT))
		{
			Transform2Df& PixToCam = m_CameraEntt.Get<Component::CameraCmp>().PixToCam;
			HVector2Df currentpos = PixToCam.QuickInverse() * HVector2Df({ (float)mouseevent.X(), (float)mouseevent.Y(), 1.0f });

			if (Core::ConsoleInput::IsKeyPressed(Core::Key::Ctrl))
			{
				Vector2Df maindir = currentpos.Vector();
				maindir = maindir / maindir.Norm();

				Transform2Df direction;

				direction.RotationBlock() = {
					maindir.x(), -maindir.y(),
					maindir.y(),  maindir.x()
				};

				Transform2Df delta = direction.QuickInverse() * m_RotationStart;

				auto& R0ToCam = m_CameraEntt.Get<Component::Transform2DCmp>().Transform;

				Transform2Df temp = R0ToCam * delta;
				R0ToCam = temp;

				m_RotationStart = direction;
			}
			else
			{
				Transform2Df end;
				end.TranslationBlock() = currentpos.Vector();

				auto& R0ToCam = m_CameraEntt.Get<Component::Transform2DCmp>().Transform;

				Transform2Df delta = end.QuickInverse() * m_TranslationStart;

				Transform2Df temp = R0ToCam * delta;
				R0ToCam = temp;

				m_TranslationStart = end;
			}

			this->SignlaUpdate.Trigger();
		}
	}
}