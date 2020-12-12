#include <iostream>
#include <thread>

#include "Devices\PaceMaker.h"
#include "Devices\Console.h"

#if TEST_CONSOLE
#error Deactivate the Test mode in Devices -> Console.h
#endif // TEST_CONSOLE

#include <../LCN_Math/Source/Utilities/Angles.h>

#include "Source\Instrumentor.h"

class Temp : public Device
{
public:
	void MainThread() override {}

	bool PContinue() { return Continue(); }
};

int main()
{
	//Instrumentor& instrumentor = Instrumentor::Get();
	Console&      console      = Console::Get();

	PaceMaker pacemaker;

	Temp temp;

	try
	{
		pacemaker.AddObserver(console);
		pacemaker.AddObserver(temp);

		HANDLE       hStdin = GetStdHandle(STD_INPUT_HANDLE);
		HWND         hWnd   = GetConsoleWindow();
		DWORD        cNumRead;
		INPUT_RECORD irInBuf[128];

		SetConsoleMode(hStdin, ENABLE_EXTENDED_FLAGS | ENABLE_MOUSE_INPUT | ENABLE_WINDOW_INPUT);

		//instrumentor.BeginSession("Test");

		Transform3D<float>& r0tocam = console.R0ToCam();

		float camspeed   = 3.0f;
		float speedboost = 1.0f;
		float anglespeed = 5.0f;
		float dt         = 16.0f / 1000.0f;

		float ax = 0.0f;
		float ay = 0.0f;

		pacemaker.Start(16ms);
		console.Start();

		RECT rect;

		GetWindowRect(hWnd, &rect);

		SHORT mouseposx = 0;
		SHORT mouseposy = 0;

		int Cx = (rect.left + rect.right) / 2;
		int Cy = (rect.top + rect.bottom) / 2;

		SetCursorPos(Cx, Cy);

		while(ShowCursor(false) >= 0);

		Transform3D<float> Tr = r0tocam;
		Transform3D<float> RotX;
		Transform3D<float> RotY;

		while (true)
		{
			temp.PContinue();

			// Closing the app
			if (GetAsyncKeyState((unsigned short)27) & 0x8000)
			{
				pacemaker.Stop();
				break;
			}

			// Pausing the app
			if (GetAsyncKeyState((unsigned short)'P') & 0x8000)
			{
				bool loop  = true;
				bool pause = false;

				pacemaker.Pause(true);

				while (loop)
				{
					cNumRead = 0;
					ReadConsoleInput(hStdin, irInBuf, 128, &cNumRead);

					for (DWORD i = 0; i < cNumRead; i++)
					{
						const INPUT_RECORD& record = irInBuf[i];

						switch (record.EventType)
						{
						case KEY_EVENT:
							if(record.Event.KeyEvent.wVirtualKeyCode == 'P')
								if (!record.Event.KeyEvent.bKeyDown)
								{
									pause = !pause;
									pacemaker.Pause(pause);

									if (!pause)
									{
										loop = false;

										GetWindowRect(hWnd, &rect);

										Cx = (rect.left + rect.right) / 2;
										Cy = (rect.top + rect.bottom) / 2;
									}
								}

							break;
						default:
							break;
						}
					}
				}

			}

			// Directionnal movements
			if (GetAsyncKeyState((unsigned short)'Z') & 0x8000)
			{
				Tr.Tx += speedboost * camspeed * r0tocam.Rwx * dt;
				Tr.Ty += speedboost * camspeed * r0tocam.Rwy * dt;
			}

			if (GetAsyncKeyState((unsigned short)'S') & 0x8000)
			{
				Tr.Tx -= speedboost * camspeed * r0tocam.Rwx * dt;
				Tr.Ty -= speedboost * camspeed * r0tocam.Rwy * dt;
			}

			if (GetAsyncKeyState((unsigned short)'Q') & 0x8000)
			{
				Tr.Tx += speedboost * camspeed * r0tocam.Rux * dt;
				Tr.Ty += speedboost * camspeed * r0tocam.Ruy * dt;
			}

			if (GetAsyncKeyState((unsigned short)'D') & 0x8000)
			{
				Tr.Tx -= speedboost * camspeed * r0tocam.Rux * dt;
				Tr.Ty -= speedboost * camspeed * r0tocam.Ruy * dt;
			}

			// Mouse movements
			cNumRead = 0;

			GetNumberOfConsoleInputEvents(hStdin, &cNumRead);

			if(cNumRead > 0)
				ReadConsoleInput(hStdin, irInBuf, 128, &cNumRead);

			for (DWORD i = 0; i < cNumRead; i++)
			{
				const INPUT_RECORD& record = irInBuf[i];

				switch (record.EventType)
				{
				case MOUSE_EVENT:
					switch (record.Event.MouseEvent.dwEventFlags)
					{
					case MOUSE_MOVED:
						static POINT pt;

						GetCursorPos(&pt);

						mouseposx = pt.x - Cx;
						mouseposy = pt.y - Cy;

						ay += mouseposx * anglespeed * dt;
						ax += mouseposy * anglespeed * dt;

						ax = std::max(std::min(ax, 80.0f), -80.0f);

						SetCursorPos(Cx, Cy);

						break;
					default:
						break;
					}

					break;
				case KEY_EVENT:
					if (record.Event.KeyEvent.dwControlKeyState & SHIFT_PRESSED)
						speedboost = 10.0f;
					else
						speedboost = 1.0f;

					break;
				default:
					break;
				}
			}

			// Compute transformation
			RotX.Rvy =  std::cos(TORAD(ax));
			RotX.Rvz =  std::sin(TORAD(ax));
			RotX.Rwy = -std::sin(TORAD(ax));
			RotX.Rwz =  std::cos(TORAD(ax));

			RotY.Rux =  std::cos(TORAD(ay));
			RotY.Ruz =  std::sin(TORAD(ay));
			RotY.Rwx = -std::sin(TORAD(ay));
			RotY.Rwz =  std::cos(TORAD(ay));

			r0tocam.mat = Tr.mat * RotY.mat * RotX.mat;
		}
	}   
	catch (const std::exception& e)
	{
		pacemaker.Stop();

		std::cout << "Critical Error : " << e.what() << std::endl;

		std::cin.get();
	}

	return 0;
}