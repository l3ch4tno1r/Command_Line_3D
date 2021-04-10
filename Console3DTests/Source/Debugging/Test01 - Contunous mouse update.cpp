#include <iostream>

#include <Console3D/Source/Core/ConsoleInput.h>

bool run = true;
std::mutex mut;
std::condition_variable cond;

class Test
{
public:
	SLOT(Test, OnMouseMove,          LCN::MouseMovedEvent&);
	SLOT(Test, OnMouseButtonPressed, LCN::MouseButtonPressedEvent&);
	SLOT(Test, OnKeyPressed,         LCN::KeyPressedEvent&);
};

void Test::OnMouseMove(LCN::MouseMovedEvent&)
{
	static int a = 0;

	auto& consoleInput = LCN::Core::ConsoleInput::Get();

	auto [Cx, Cy] = consoleInput.GetWindowCenter();

	std::cout << "Mouse event #" << a++ << std::endl;

	consoleInput.SetCursorPosition(Cx, Cy);
}

void Test::OnKeyPressed(LCN::KeyPressedEvent& keyEvent)
{
	std::cout << "Key pressed !" << std::endl;

	if (keyEvent.KeyCode() == static_cast<unsigned short>(LCN::Core::Key::Esc))
	{
		std::lock_guard<std::mutex> lock(mut);

		run = false;

		cond.notify_one();
	}
}

void Test::OnMouseButtonPressed(LCN::MouseButtonPressedEvent&)
{
	std::cout << "Button clicked !" << std::endl;
}

int main()
{
	Test test;

	auto& consoleInput = LCN::Core::ConsoleInput::Get();

	LCN::Connect(consoleInput.SignalMouseMove,          test.SlotOnMouseMove);
	LCN::Connect(consoleInput.SignalKeyPressed,         test.SlotOnKeyPressed);
	LCN::Connect(consoleInput.SignalMouseButtonPressed, test.SlotOnMouseButtonPressed);

	consoleInput.Start();

	[]()
	{
		std::unique_lock<std::mutex> lock(mut);

		while (run)
			cond.wait(lock);
	}();
}
