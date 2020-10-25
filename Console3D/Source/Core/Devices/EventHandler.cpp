#include "EventHandler.h"

EventHandler& EventHandler::Get() noexcept
{
	static EventHandler instance;
	return instance;
}

EventHandler::EventHandler()
{
	m_HStdIn = GetStdHandle(STD_INPUT_HANDLE);
}

void EventHandler::Start()
{
	if (m_HStdIn == INVALID_HANDLE_VALUE)
		throw std::exception("Handle invalid.");

	if(!SetConsoleMode(m_HStdIn, ENABLE_EXTENDED_FLAGS | ENABLE_MOUSE_INPUT | ENABLE_WINDOW_INPUT);)
		throw std::exception("Cannot set console mode.");
}

void EventHandler::MainThread()
{
	DWORD        cNumRead;
	INPUT_RECORD irInBuf[128];
}
