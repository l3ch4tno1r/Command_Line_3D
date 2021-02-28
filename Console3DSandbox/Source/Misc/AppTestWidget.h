#pragma once

#include <Console3D/Source/Core/EventApplication.h>
#include <Console3D/Source/Gui/Widget.h>

namespace LCN
{
	class AppTestWidget : public EventApplication
	{
	public:
		AppTestWidget() = default;

	private:
		CWidget m_Widget;
		CWidget m_Widget2;
	};
}

using LCNApp = LCN::AppTestWidget;