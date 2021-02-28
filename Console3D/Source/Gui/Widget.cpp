#include <exception>

#include "Widget.h"

#include "Console3D/Source/Core/Application.h"

namespace LCN
{
	CWidget::CWidget()
	{
		Application::Get();
	}
}
