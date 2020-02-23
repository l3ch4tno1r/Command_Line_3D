#include "Console.h"

#include <Utilities\ErrorHandling.h>

#include <chrono>
#include <cmath>

#define CONSOLETEST02

Console::Console() :
	m_Width(180),
	m_Height(120),
	screen(nullptr),
	hConsole(nullptr),
	dwBytesWritten(0)
{
	screen = new char[m_Width * m_Height];
	hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
}

Console::~Console()
{
	delete[] screen;
}

Console& Console::Get()
{
	static Console console;
	return console;
}

void Console::Clear()
{
	for (unsigned int i = 0; i < m_Width * m_Height; i++)
		screen[i] = 0;
}

void Console::DrawPoint(float x, float y, char c)
{
	//ASSERT(x >= 0 && x < width);
	//ASSERT(y >= 0 && y < height);

	if (x < 0 || x >= m_Width)
		return;

	if (y < 0 || y >= m_Height)
		return;

	screen[(INT32)x + (INT32)y * m_Width] = c;
}

#ifdef CONSOLETEST01
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
	//float alpha = 180 * atan2(y2 - y1, x2 - x1) / PI;
	//
	//UINT8 idxmin = 0, idxmax = 8;
	//
	//while ((idxmax - idxmin) > 1)
	//{
	//	UINT8 mid = (idxmax + idxmin) / 2;
	//
	//	if (alpha >= intervals[mid])
	//		idxmin = mid;
	//	else
	//		idxmax = mid;
	//}

	//--//
	if (x1 > x2)
	{
		float temp = x1;
		x1 = x2;
		x2 = temp;

		temp = y1;
		y1 = y2;
		y2 = temp;
	}

	float dx = x2 - x1;
	float dy = y2 - y1;
	float norm = sqrt(dx * dx + dy * dy);

	float udx = dx / norm;
	float udy = dy / norm;

	float fX1 = floor(x1);
	float fY1 = floor(y1);

	float _X1 = fX1 + 0.5 + udx;
	float _Y1 = fY1 + 0.5 + udy;

	UINT8 idxmin;

	if (_X1 < fX1)
	{
		if (_Y1 < fY1)
			idxmin = Card::NO;
		else if (_Y1 > fY1 + 1)
			idxmin = Card::SO;
		else
			idxmin = Card::O;
	}
	else if (_X1 > fX1 + 1)
	{
		if (_Y1 < fY1)
			idxmin = Card::NE;
		else if (_Y1 > fY1 + 1)
			idxmin = Card::SE;
		else
			idxmin = Card::E;
	}
	else
	{

		if (_Y1 < fY1)
			idxmin = Card::N;
		else if (_Y1 > fY1 + 1)
			idxmin = Card::S;
	}
	
	//---------------//
	float a = dy;
	float b = -dx;
	float c = -((a * x1) + (b * y1));

	float X = x1;
	float Y = y1;

	while ((abs(X - x1) + abs(Y - y1)) < (abs(x2 - x1) + abs(y2 - y1)))
	{
		float min = 100000;
		INT8  idx = -1;

		for (UINT8 i = 0; i < 5; i++)
		{
			UINT8 I = (i + idxmin - 2) % 8;

			float eval = abs(a * (floor(X) + 0.5 + neighboors[I][0]) + b * (floor(Y) + 0.5 + neighboors[I][1]) + c);

			if (eval <= min)
			{
				min = eval;
				idx = I;
			}
		}

		X += neighboors[idx][0];
		Y += neighboors[idx][1];

		DrawPoint(X, Y);
	}

	DrawPoint(x1, y1);
	DrawPoint(x2, y2);
}
#endif

#ifdef CONSOLETEST02
void Console::DrawLine(float x1, float y1, float x2, float y2)
{
	float dx = x2 - x1;
	float dy = y2 - y1;

	if (dx == 0 && dy == 0)
	{
		DrawPoint(x1, y1);
		return;
	}

	float stepx, stepy;

	if (abs(dx) > abs(dy))
	{
		stepx = (dx > 0 ? 1.0f : -1.0f);
		stepy = dy / abs(dx);
	}
	else
	{
		stepx = dx / abs(dy);
		stepy = (dy > 0 ? 1.0f : -1.0f);
	}

	float X = x1;
	float Y = y1;

	while ((abs(X - x1) + abs(Y - y1)) < (abs(x2 - x1) + abs(y2 - y1)))
	{
		X += stepx;
		Y += stepy;

		DrawPoint(X, Y);
	}

	DrawPoint(x1, y1);
	DrawPoint(x2, y2);
}
#endif

void Console::DisplayMessage(const std::string & msg)
{
	for (UINT32 i = 0; i < msg.length(); i++)
		DrawPoint(i + 1, m_Height - 2, msg[i]);
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
		DrawPoint(m_Width - 2, m_Height - 2, '0');
	else
		DrawPoint(m_Width - 2, m_Height - 2, ' ');
}

void Console::Render()
{
	screen[m_Width * m_Height - 1] = '\0';
	WriteConsoleOutputCharacter(hConsole, screen, m_Width * m_Height, { 0,0 }, &dwBytesWritten);
}

// Instanciation
//Console Console::console;