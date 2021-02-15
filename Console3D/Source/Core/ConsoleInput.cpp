#include "ConsoleInput.h"

#ifdef DEBUG
#include <iostream>
#include <bitset>
#define EVENT_DEBUG(X) std::cout << X << std::endl
#else
#define EVENT_DEBUG(X)
#endif // DEBUG

namespace LCN
{
	ConsoleInput& ConsoleInput::Get() noexcept
	{
		static ConsoleInput instance;
		return instance;
	}

	bool ConsoleInput::IsKeyPressed(Key key)
	{
		return Get().m_KeysState[key].KeyHeld;
	}

	bool ConsoleInput::IsMouseBtnPressed(MouseButton mousebtn)
	{
		return Get().m_Mouse[mousebtn].KeyHeld;
	}

	ConsoleInput::ConsoleInput()
	{
		m_HStdIn = GetStdHandle(STD_INPUT_HANDLE);

		std::memset(m_KeysState,  0, 256 * sizeof(KeyState));
		std::memset(m_Mouse, 0,   5 * sizeof(KeyState));
	}

	ConsoleInput::~ConsoleInput()
	{
		m_Run = false;

		if (m_MainThread.joinable())
			m_MainThread.join();
	}

	void ConsoleInput::Start()
	{
		if (m_HStdIn == INVALID_HANDLE_VALUE)
			throw std::exception("Handle invalid.");

		if(!SetConsoleMode(m_HStdIn, ENABLE_EXTENDED_FLAGS | ENABLE_MOUSE_INPUT | ENABLE_WINDOW_INPUT))
			throw std::exception("Cannot set console mode.");

		m_Run = true;

		m_MainThread = std::thread(&ConsoleInput::MainThread, this);
	}

	void ConsoleInput::Stop()
	{
		m_Run = false;
	}

	void ConsoleInput::MainThread()
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
					KeyState& key = m_KeysState[record.Event.KeyEvent.wVirtualKeyCode];

					key.KeyNewState =  record.Event.KeyEvent.bKeyDown;
					key.KeyHeld     =  key.KeyNewState;
					key.KeyPressed  = !key.KeyOldState &&  key.KeyNewState;
					key.KeyReleased =  key.KeyOldState && !key.KeyNewState;

					// Key Pressed
					if (key.KeyPressed)
					{
						KeyPressedEvent keypressedevent(record.Event.KeyEvent.wVirtualKeyCode);
						this->SignalKeyPressed.Trigger(keypressedevent);
					}

					// Key Released
					if (key.KeyReleased)
					{
						KeyReleasedEvent keyreleasedevent(record.Event.KeyEvent.wVirtualKeyCode);
						this->SignalKeyReleased.Trigger(keyreleasedevent);
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
						MouseMovedEvent mousemoveevent(x, y);
						this->SignalMouseMove.Trigger(mousemoveevent);

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

							// Mouse button pressed signal
							if (m_Mouse[i].KeyPressed)
							{
								MouseButtonPressedEvent mousebuttonpressed(x, y, i);
								this->SignalMouseButtonPressed.Trigger(mousebuttonpressed);
							}

							// Mouse button released signal
							if (m_Mouse[i].KeyReleased)
							{
								MouseButtonReleasedEvent mousebuttonreleased(x, y, i);
								this->SignalMouseButtonReleased.Trigger(mousebuttonreleased);
							}

							m_Mouse[i].KeyOldState = m_Mouse[i].KeyNewState;
						}

						break;
					}
					case MOUSE_WHEELED:
					{
						long dir = record.Event.MouseEvent.dwButtonState & dwButtonStateHighWordMask;

						MouseScrollEvent mousescrollevent(x, y, sign(dir));
						this->SignalMouseScroll.Trigger(mousescrollevent);

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