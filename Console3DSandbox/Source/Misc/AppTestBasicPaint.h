#pragma once

#include <Console3D/Source/Core/Application.h>
#include <Console3D/Source/Gui/Widget.h>

#include <LCN_Math/Source/Geometry/Vector.h>

namespace LCN
{
	class AppTestBasicPaint : public Application
	{
	#define APPWIDGET_SLOT(Method, ...) SLOT(AppTestBasicPaint, Method, __VA_ARGS__)

	public:
		AppTestBasicPaint();

		void Run() override;

	private: // Slots
		APPWIDGET_SLOT(OnKeyPessed, KeyPressedEvent&);

		APPWIDGET_SLOT(OnMouseButtonPressed,  MouseButtonPressedEvent&);
		APPWIDGET_SLOT(OnMouseButtonReleased, MouseButtonReleasedEvent&);
		APPWIDGET_SLOT(OnMouseMoved,          MouseMovedEvent&);

	private:
		bool m_DrawingMode = false;

		Vector2Di m_LastPoint;
	};
}

using LCNApp = LCN::AppTestBasicPaint;