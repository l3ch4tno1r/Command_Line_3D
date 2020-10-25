#pragma once

#define NOMINMAX
#include <Windows.h>

#include "Device.h"

struct KeyState
{
	bool KeyPressed  : 1;
	bool KeyReleased : 1;
	bool KeyHeld     : 1;
	bool _NA         : 3;
	bool KeyOldState : 1;
	bool KeyNewState : 1;
};

using KeyBoardAction = std::function<void(const KeyState&)>;
using MouseAction    = std::function<void(const KeyState&, int, int)>;

class EventHandler : public Device
{
public:
	static EventHandler& Get() noexcept;

	void Start() override;

	void AddKeyBoardAction(size_t keyid, KeyBoardAction&& action);

private:
	EventHandler();

	void MainThread() override;

private:
	HANDLE m_HStdIn;

	KeyState m_Keys[256], m_Mouse[5];

	KeyBoardAction m_KeyActions[256];
	MouseAction    m_MouseActions[5];
};