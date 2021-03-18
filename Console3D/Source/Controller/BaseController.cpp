#include "BaseController.h"

#include "Console3D/Source/Core/ConsoleInput.h"

namespace LCN::Controller
{
	BaseController::BaseController()
	{
		auto& consoleInput = Core::ConsoleInput::Get();

		Connect(consoleInput.SignalKeyPressed,         SlotOnKeyPressed);
		Connect(consoleInput.SignalMouseMove,          SlotOnMouseMove);
		Connect(consoleInput.SignalMouseButtonPressed, SlotOnMouseButtonPressed);
		Connect(consoleInput.SignalMouseScroll,        SlotOnMouseScroll);
	}
}
