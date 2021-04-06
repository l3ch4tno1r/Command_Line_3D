#include "ConsoleInput.h"

#ifdef DEBUG
#include <iostream>
#include <bitset>
#define EVENT_DEBUG(X) std::cout << X << std::endl
#else
#define EVENT_DEBUG(X)
#endif // DEBUG

namespace LCN::Core
{
	ConsoleInput& ConsoleInput::Get() noexcept
	{
		static ConsoleInput instance;
		return instance;
	}

	bool ConsoleInput::IsKeyPressed(Key key)
	{
		return Get().m_KeysState[static_cast<size_t>(key)].KeyHeld;
	}

	bool ConsoleInput::IsMouseBtnPressed(MouseButton mousebtn)
	{
		return Get().m_Mouse[mousebtn].KeyHeld;
	}

	std::tuple<int, int> ConsoleInput::GetWindowCenter() const
	{
		RECT rect;

		GetWindowRect(m_HWnd, &rect);

		int Cx = (rect.left + rect.right) / 2;
		int Cy = (rect.top + rect.bottom) / 2;

		return { Cx, Cy };
	}

	void ConsoleInput::SetCursorPosition(int x, int y)
	{
		SetCursorPos(x, y);
	}

	void ConsoleInput::Continue()
	{
		std::lock_guard<std::mutex> lock(m_ContinueMut);

		m_Notified = true;

		m_ContinueCond.notify_one();
	}

	void ConsoleInput::Wait()
	{
		std::unique_lock<std::mutex> lock(m_ContinueMut);

		while (!m_Notified)
			m_ContinueCond.wait(lock);

		m_Notified = false;
	}

	ConsoleInput::ConsoleInput()
	{
		m_HStdIn = GetStdHandle(STD_INPUT_HANDLE);
		m_HWnd   = GetConsoleWindow();

		std::memset(m_KeysState,  0, 256 * sizeof(KeyState));
		std::memset(m_Mouse,      0,   5 * sizeof(KeyState));
	}

	ConsoleInput::~ConsoleInput()
	{
		m_Run = false;

		this->Continue();

		if (m_MainThread.joinable())
			m_MainThread.join();
	}

	void ConsoleInput::Start()
	{
		if (m_HStdIn == INVALID_HANDLE_VALUE)
			throw std::exception("Handle invalid.");

		if (m_HWnd == INVALID_HANDLE_VALUE)
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
					POINT pt;

					GetCursorPos(&pt);

					switch (record.Event.MouseEvent.dwEventFlags)
					{
					case MOUSE_MOVED:
					{
						MouseMovedEvent mousemoveevent(pt.x, pt.y);
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
								MouseButtonPressedEvent mousebuttonpressed(pt.x, pt.y, i);
								this->SignalMouseButtonPressed.Trigger(mousebuttonpressed);
							}

							// Mouse button released signal
							if (m_Mouse[i].KeyReleased)
							{
								MouseButtonReleasedEvent mousebuttonreleased(pt.x, pt.y, i);
								this->SignalMouseButtonReleased.Trigger(mousebuttonreleased);
							}

							m_Mouse[i].KeyOldState = m_Mouse[i].KeyNewState;
						}

						break;
					}
					case MOUSE_WHEELED:
					{
						long dir = record.Event.MouseEvent.dwButtonState & dwButtonStateHighWordMask;

						MouseScrollEvent mousescrollevent(pt.x, pt.y, sign(dir));
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

			//this->Wait();
		}
	}
}