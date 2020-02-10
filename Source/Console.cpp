#include "Console.h"

#include <Utilities\ErrorHandling.h>

#include <cmath>

#define PI 3.14159265

Console::Console() :
	width(120),
	height(80),
	screen(nullptr),
	hConsole(nullptr),
	dwBytesWritten(0)
{
	screen = new char[width * height];
	hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
}

Console::~Console()
{
	delete[] screen;
}

Console& Console::Get()
{
	return console;
}

void Console::Clear()
{
	for (unsigned int i = 0; i < width * height; i++)
		screen[i] = 0;
}

void Console::DrawPoint(float x, float y, char c)
{
	//ASSERT(x >= 0 && x < width);
	//ASSERT(y >= 0 && y < height);

	if (x < 0 || x >= width)
		return;

	if (y < 0 || y >= height)
		return;

	screen[(INT32)x + (INT32)y * width] = c;
}

void Console::DrawLine(float x1, float y1, float x2, float y2)
{
	//static const int intervals[] = { -180, -135, -90, -45, 0, 45, 90, 135, 180 };
	//
	//float a = 180 * atan2(y2 - y1, x2 - x1) / PI;
	//
	//UINT8 idxmin = 0, idxmax = 8;
	//
	//while ((idxmax - idxmin) > 1)
	//{
	//	UINT8 mid = (idxmax + idxmin) / 2;
	//
	//	if (a >= intervals[mid])
	//		idxmin = mid;
	//	else
	//		idxmax = mid;
	//}

	//float dx = x2 - x1;
	//float dy = y2 - y1;
	//
	//float a = dy;
	//float b = -dx;
	//float c = -((a * x1) + (b * y1));

	int X1 = x1;
	int Y1 = y1;
	int X2 = x2;
	int Y2 = y2;

	int DX = X2 - X1;
	int DY = Y2 - Y1;

	float r = (float)DY / (float)DX;

	int X = X1;
	int Y = Y1;

	int sx = (0 < DX) - (DX < 0);

	while (X != X2)
	{
		X += sx;
		Y = Y1 + r * X;

		DrawPoint(X, Y);
	}
}

void Console::DisplayMessage(const std::string & msg)
{
	for (UINT32 i = 0; i < msg.length(); i++)
		DrawPoint(i + 1, height - 2, msg[i]);
}

void Console::Render()
{
	screen[width * height - 1] = '\0';
	WriteConsoleOutputCharacter(hConsole, screen, width * height, { 0,0 }, &dwBytesWritten);
}

// Instanciation
Console Console::console;