#include "BaseController.h"

#include "ConsoleInput.h"

namespace LCN::Controller
{
	BaseController::BaseController()
	{
		auto& consoleInput = Core::ConsoleInput::Get();

		Connect(consoleInput.SignalKeyPressed,         this->SlotOnKeyPressed);
		Connect(consoleInput.SignalKeyReleased,        this->SlotOnKeyReleased);
		Connect(consoleInput.SignalMouseMove,          this->SlotOnMouseMove);
		Connect(consoleInput.SignalMouseButtonPressed, this->SlotOnMouseButtonPressed);
		Connect(consoleInput.SignalMouseScroll,        this->SlotOnMouseScroll);
	}
}
