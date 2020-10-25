#pragma once

#define NOMINMAX
#include <Windows.h>

#include "Device.h"

class EventHandler : public Device
{
public:
	static EventHandler& Get() noexcept;

	void Start() override;

private:
	EventHandler();

	void MainThread() override;

private:
	HANDLE m_HStdIn;
};