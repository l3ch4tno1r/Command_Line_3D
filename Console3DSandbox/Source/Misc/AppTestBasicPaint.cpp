#include "AppTestBasicPaint.h"

#include <Console3D/Source/Core/Console.h>

namespace LCN
{
	AppTestBasicPaint::AppTestBasicPaint() :
		SLOT_INIT(SlotOnKeyPessed,           AppTestBasicPaint::OnKeyPessed),
		SLOT_INIT(SlotOnMouseButtonPressed,  AppTestBasicPaint::OnMouseButtonPressed),
		SLOT_INIT(SlotOnMouseButtonReleased, AppTestBasicPaint::OnMouseButtonReleased),
		SLOT_INIT(SlotOnMouseMoved,          AppTestBasicPaint::OnMouseMoved)
	{
		Connect(this->SignalKeyPressed,          this->SlotOnKeyPessed);
		Connect(this->SignalMouseButtonPressed,  this->SlotOnMouseButtonPressed);
		Connect(this->SignalMouseButtonReleased, this->SlotOnMouseButtonReleased);
		Connect(this->SignalMouseMoved,          this->SlotOnMouseMoved);

		Console::Get().ConstructConsole(300, 200, 4, 4);
		Console::Get().Clear();
	}

	void AppTestBasicPaint::Run()
	{
		this->WaitQuit();
	}

	void AppTestBasicPaint::OnMouseButtonPressed(MouseButtonPressedEvent& mouseevent)
	{
		if (mouseevent.ButtonCode() == MouseButton::LEFT)
		{
			m_DrawingMode = true;

			m_LastPoint.x() = mouseevent.X();
			m_LastPoint.y() = mouseevent.Y();

			Console::Get().DrawPoint(mouseevent.X(), mouseevent.Y());
			Console::Get().Render();
		}
	}

	void AppTestBasicPaint::OnMouseButtonReleased(MouseButtonReleasedEvent&)
	{
		m_DrawingMode = false;
	}

	void AppTestBasicPaint::OnMouseMoved(MouseMovedEvent& mouseevent)
	{
		if (m_DrawingMode)
		{
			Console::Get().DrawLine(
				mouseevent.X(),  mouseevent.Y(),
				m_LastPoint.x(), m_LastPoint.y());

			Console::Get().Render();

			m_LastPoint.x() = mouseevent.X();
			m_LastPoint.y() = mouseevent.Y();
		}
	}

	void AppTestBasicPaint::OnKeyPessed(KeyPressedEvent& keyevent)
	{
		switch (keyevent.KeyCode())
		{
		case Key::Esc:
			this->Quit();
			break;
		case Key::E:
			Console::Get().Clear();
			Console::Get().Render();
			break;
		default:
			break;
		}
	}
}
