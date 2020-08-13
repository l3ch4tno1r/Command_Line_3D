#include <iostream>

#define NOMINMAX
#include <Windows.h>

#ifndef UNICODE
#error Please enable UNICODE for your compiler ! VS : \
Project Properties -> General -> Character Set -> Use Unicode.
#endif

enum COLOUR
{
	FG_BLACK        = 0x0000,
	FG_DARK_BLUE    = 0x0001,
	FG_DARK_GREEN   = 0x0002,
	FG_DARK_CYAN    = 0x0003,
	FG_DARK_RED     = 0x0004,
	FG_DARK_MAGENTA = 0x0005,
	FG_DARK_YELLOW  = 0x0006,
	FG_GREY         = 0x0007,
	FG_DARK_GREY    = 0x0008,
	FG_BLUE         = 0x0009,
	FG_GREEN        = 0x000A,
	FG_CYAN         = 0x000B,
	FG_RED          = 0x000C,
	FG_MAGENTA      = 0x000D,
	FG_YELLOW       = 0x000E,
	FG_WHITE        = 0x000F,
	BG_BLACK        = 0x0000,
	BG_DARK_BLUE    = 0x0010,
	BG_DARK_GREEN   = 0x0020,
	BG_DARK_CYAN    = 0x0030,
	BG_DARK_RED     = 0x0040,
	BG_DARK_MAGENTA = 0x0050,
	BG_DARK_YELLOW  = 0x0060,
	BG_GREY         = 0x0070,
	BG_DARK_GREY    = 0x0080,
	BG_BLUE         = 0x0090,
	BG_GREEN        = 0x00A0,
	BG_CYAN         = 0x00B0,
	BG_RED          = 0x00C0,
	BG_MAGENTA      = 0x00D0,
	BG_YELLOW       = 0x00E0,
	BG_WHITE        = 0x00F0,
};

enum PIXEL_TYPE
{
	PIXEL_SOLID         = 0x2588,
	PIXEL_THREEQUARTERS = 0x2593,
	PIXEL_HALF          = 0x2592,
	PIXEL_QUARTER       = 0x2591,
};

int main()
{
	try
	{
		HANDLE hconsole = GetStdHandle(STD_OUTPUT_HANDLE);

		if (hconsole == INVALID_HANDLE_VALUE)
			throw std::exception("Handle invalid.");

		int width  = 50;
		int height = 50;
		int fontw  = 16;
		int fonth  = 16;

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

		// Testing
		PCHAR_INFO screenbuffer = new CHAR_INFO[width * height];

		std::memset(screenbuffer, 0, width * height * sizeof(CHAR_INFO));

		int x = width / 2;
		int y = height / 2;

		screenbuffer[x + y * width].Char.UnicodeChar = PIXEL_TYPE::PIXEL_SOLID;
		screenbuffer[x + y * width].Attributes       = COLOUR::BG_WHITE;

		screenbuffer[x + 1 + y * width].Char.UnicodeChar = PIXEL_TYPE::PIXEL_THREEQUARTERS;
		screenbuffer[x + 1 + y * width].Attributes       = COLOUR::BG_WHITE;

		screenbuffer[x + 2 + y * width].Char.UnicodeChar = PIXEL_TYPE::PIXEL_HALF;
		screenbuffer[x + 2 + y * width].Attributes       = COLOUR::BG_WHITE;

		screenbuffer[x + 3 + y * width].Char.UnicodeChar = PIXEL_TYPE::PIXEL_QUARTER;
		screenbuffer[x + 3 + y * width].Attributes       = COLOUR::BG_WHITE;

		screenbuffer[x + 4 + y * width].Char.UnicodeChar = 0;
		screenbuffer[x + 4 + y * width].Attributes       = COLOUR::BG_WHITE;

		WriteConsoleOutput(hconsole, screenbuffer, { (short)width, (short)height }, { 0,0 }, &rect);

		delete[] screenbuffer;
	}
	catch (const std::exception& e)
	{
		std::cout << "Exception : " << e.what() << std::endl;
	}

	std::cin.get();
}