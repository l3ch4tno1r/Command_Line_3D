#pragma once

#define NOMINMAX
#include <Windows.h>

#include <string>
#include <thread>

class Console
{
private:
	//static Console console;

	UINT32 m_Width;
	UINT32 height;

	char*  screen;
	HANDLE hConsole;
	DWORD  dwBytesWritten;

	std::thread m_MainThread;

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