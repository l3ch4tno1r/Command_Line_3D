#include "EventHandler.h"

#ifdef DEBUG
#include <iostream>
#define EVENT_DEBUG(X) std::cout << X << std::endl
#else
#define EVENT_DEBUG(X)
#endif // DEBUG

namespace LCN
{
	EventHandler& EventHandler::Get() noexcept
	{
		static EventHandler instance;
		return instance;
	}

	EventHandler::EventHandler()
	{
		m_HStdIn = GetStdHandle(STD_INPUT_HANDLE);

		std::memset(m_Keys,  0, 256 * sizeof(KeyState));
		std::memset(m_Mouse, 0,   5 * sizeof(KeyState));
	}

	EventHandler::~EventHandler()
	{
		m_Run = false;

		if (m_MainThread.joinable())
			m_MainThread.join();
	}

	void EventHandler::Start()
	{
		if (m_HStdIn == INVALID_HANDLE_VALUE)
			throw std::exception("Handle invalid.");

		if(!SetConsoleMode(m_HStdIn, ENABLE_EXTENDED_FLAGS | ENABLE_MOUSE_INPUT | ENABLE_WINDOW_INPUT))
			throw std::exception("Cannot set console mode.");

		m_Run = true;

		m_MainThread = std::thread(&EventHandler::MainThread, this);
	}

	void EventHandler::Stop()
	{
		m_Run = false;
	}

	void EventHandler::MainThread()
	{
		DWORD        cNumRead;
		INPUT_RECORD irInBuf[128];

		while (m_Run)
		{
			cNumRead = 0;
			ReadConsoleInput(m_HStdIn, irInBuf, 128, &cNumRead);

			for (DWORD i = 0; i < cNumRead; i++)
			{
				const INPUT_RECORD& record = irInBuf[i];

				switch (record.EventType)
				{
				case KEY_EVENT:
				{
					KeyState& key = m_Keys[record.Event.KeyEvent.wVirtualKeyCode];

					key.KeyNewState =  record.Event.KeyEvent.bKeyDown;
					key.KeyHeld     =  key.KeyNewState;
					key.KeyPressed  = !key.KeyOldState &&  key.KeyNewState;
					key.KeyReleased =  key.KeyOldState && !key.KeyNewState;

					// Key Pressed
					if (key.KeyPressed)
					{
						KeyPressedEvent keypressedevent(record.Event.KeyEvent.wVirtualKeyCode);
						this->SignalKeyPressed.Emmit(keypressedevent);
					}

					// Key Released
					if (key.KeyReleased)
					{
						KeyReleasedEvent keyreleasedevent(record.Event.KeyEvent.wVirtualKeyCode);
						this->SignalKeyReleased.Emmit(keyreleasedevent);
					}

					key.KeyOldState = key.KeyNewState;

					break;
				}
				case MOUSE_EVENT:
				{
					int x = record.Event.MouseEvent.dwMousePosition.X;
					int y = record.Event.MouseEvent.dwMousePosition.Y;

					switch (record.Event.MouseEvent.dwEventFlags)
					{
					case MOUSE_MOVED:
					{
						if(m_MouseMoveAction)
							m_MouseMoveAction(x, y);

						break;
					}
					case 0: // Button clicked
					{
						for (int i = 0; i < 5; ++i)
						{
							m_Mouse[i].KeyNewState = (record.Event.MouseEvent.dwButtonState & (1 << i)) > 0;
							m_Mouse[i].KeyHeld     =  m_Mouse[i].KeyNewState;
							m_Mouse[i].KeyPressed  = !m_Mouse[i].KeyOldState &&  m_Mouse[i].KeyNewState;
							m_Mouse[i].KeyReleased =  m_Mouse[i].KeyOldState && !m_Mouse[i].KeyNewState;

							if (m_MouseActions[i])
								m_MouseActions[i](m_Mouse[i], x, y);

							m_Mouse[i].KeyOldState = m_Mouse[i].KeyNewState;
						}

						break;
					}
					default:
						break;
					}

					break;
				}
				default:
					break;
				}
			}
		}
	}
}