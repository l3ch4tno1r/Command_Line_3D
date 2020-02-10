#pragma once

#include <Windows.h>

#include <string>

#define PI 3.14159265
#define TORAD(A) (PI * A) / 180

class Console
{
private:
	static Console console;

	UINT32 width;
	UINT32 height;

	char*  screen;
	HANDLE hConsole;
	DWORD  dwBytesWritten;

	Console();

	~Console();

	Console(const Console&) = delete;
	Console& operator=(const Console&) = delete;

public:
	static Console& Get();

	void Clear();

	void DrawPoint(float x, float y, char c = '#');

	void DrawLine(float x1, float y1, float x2, float y2);

	void DisplayMessage(const std::string& msg);

	void HeartBeat();

	void Render();
};