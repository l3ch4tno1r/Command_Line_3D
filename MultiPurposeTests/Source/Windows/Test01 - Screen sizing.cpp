#include <iostream>

#define NOMINMAX
#include <Windows.h>

int main()
{
	try
	{
		HANDLE hconsole = GetStdHandle(STD_OUTPUT_HANDLE);

		if (hconsole == INVALID_HANDLE_VALUE)
			throw std::exception("Handle invalid.");

		int width  = 200;
		int height = 200;
		int fontw  = 4;
		int fonth  = 4;

		SMALL_RECT rect = { 0, 0, 1, 1 };

		SetConsoleWindowInfo(hconsole, TRUE, &rect);

		COORD coord = { (short)width, (short)height };

		if (!SetConsoleScreenBufferSize(hconsole, coord))
			throw std::exception("Cannot set Console screen buffer size.");

		if(!SetConsoleActiveScreenBuffer(hconsole))
			throw std::exception("Cannot set Console active screen buffer.");

		rect = { 0, 0, (short)width - 1, (short)height - 1 };

		CONSOLE_FONT_INFOEX cfi;

		cfi.cbSize       = sizeof(cfi);
		cfi.nFont        = 0;
		cfi.dwFontSize.X = fontw;
		cfi.dwFontSize.Y = fonth;
		cfi.FontFamily   = FF_DONTCARE;
		cfi.FontWeight   = FW_NORMAL;

		wcscpy_s(cfi.FaceName, L"Consolas");

		if (!SetCurrentConsoleFontEx(hconsole, FALSE, &cfi))
			throw std::exception("Cannot set Console font.");

		CONSOLE_SCREEN_BUFFER_INFO csbi;

		if (!GetConsoleScreenBufferInfo(hconsole, &csbi))
			throw std::exception("Cannot get Console screen buffer info.");

		if (width > csbi.dwMaximumWindowSize.X)
			throw std::exception("Screen Width / Font Width Too Big.");

		if (height > csbi.dwMaximumWindowSize.Y)
			throw std::exception("Screen Height / Font Height Too Big.");

		// Set console size
		if (!SetConsoleWindowInfo(hconsole, TRUE, &rect))
			throw std::exception("Cannot set Console physical size.");
	}
	catch (const std::exception& e)
	{
		std::cout << "Exception : " << e.what() << std::endl;
	}

	std::cin.get();
}