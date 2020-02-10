#include "Console.h"

#include <Utilities\ErrorHandling.h>

#include <chrono>
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
	static const int intervals[] = { -180, -135, -90, -45, 0, 45, 90, 135, 180 };
	
	static const int neighboors[8][2] = {
		{ -1,   0 },
		{ -1,  -1 },
		{  0,  -1 },
		{  1,  -1 },
		{  1,   0 },
		{  1,   1 },
		{  0,   1 },
		{ -1,   1 }
	};

	enum Card
	{
		O,
		NO,
		N,
		NE,
		E,
		SE,
		S,
		SO
	};

	// Look for main direction
	float alpha = 180 * atan2(y2 - y1, x2 - x1) / PI;
	
	UINT8 idxmin = 0, idxmax = 8;
	
	while ((idxmax - idxmin) > 1)
	{
		UINT8 mid = (idxmax + idxmin) / 2;
	
		if (alpha >= intervals[mid])
			idxmin = mid;
		else
			idxmax = mid;
	}

	//--//
	float dx = x2 - x1;
	float dy = y2 - y1;
	
	float a = dy;
	float b = -dx;
	float c = -((a * x1) + (b * y1));

	float X = x1;
	float Y = y1;

	while (floor(X) != floor(x2) && floor(Y) != floor(y2))
	{
		float min = 100000;
		INT8  idx = -1;

		for (UINT8 i = 0; i < 3; i++)
		{
			UINT8 I = (i + idxmin - 1) % 8;

			float eval = a * (floor(x1) + 0.5 + neighboors[I][0]) + b * (floor(y1) + 0.5 + neighboors[I][1]) + c;

			if (abs(eval) < abs(min))
			{
				min = eval;
				idx = I;
			}
		}

		X += neighboors[idx][0];
		Y += neighboors[idx][1];

		DrawPoint(X, Y);
	}

	DrawPoint(x2, y2);
}

void Console::DisplayMessage(const std::string & msg)
{
	for (UINT32 i = 0; i < msg.length(); i++)
		DrawPoint(i + 1, height - 2, msg[i]);
}

void Console::HeartBeat()
{
	static auto start = std::chrono::high_resolution_clock::now();

	auto end = std::chrono::high_resolution_clock::now();
	INT64 ellapsed_time = std::chrono::duration_cast<std::chrono::milliseconds> (end - start).count();

	if (ellapsed_time > 2000)
	{
		start = std::chrono::high_resolution_clock::now();

		return;
	}

	if (ellapsed_time < 1000)
		DrawPoint(width - 2, height - 2, '0');
	else
		DrawPoint(width - 2, height - 2, ' ');
}

void Console::Render()
{
	screen[width * height - 1] = '\0';
	WriteConsoleOutputCharacter(hConsole, screen, width * height, { 0,0 }, &dwBytesWritten);
}

// Instanciation
Console Console::console;