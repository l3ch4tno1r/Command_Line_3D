#include <iostream>
#include <thread>

#include "Devices/Console.h"

int main()
{
	bool run = true;

	HANDLE       hStdin = GetStdHandle(STD_INPUT_HANDLE);
	HWND         hWnd = GetConsoleWindow();
	DWORD        cNumRead;
	INPUT_RECORD irInBuf[128];

	SetConsoleMode(hStdin, ENABLE_EXTENDED_FLAGS | ENABLE_MOUSE_INPUT | ENABLE_WINDOW_INPUT);

	Console& console = Console::Get();
	console.Start();

	while (run)
	{
		cNumRead = 0;
		ReadConsoleInput(hStdin, irInBuf, 128, &cNumRead);

		for (DWORD i = 0; i < cNumRead; i++)
		{
			const INPUT_RECORD& record = irInBuf[i];

			switch (record.EventType)
			{
			case KEY_EVENT:
				switch (record.Event.KeyEvent.wVirtualKeyCode)
				{
				case VK_ESCAPE:
					run = false;

					break;
				default:
					break;
				}

				break;
			default:
				break;
			}
		}
	}

	return EXIT_SUCCESS;
}