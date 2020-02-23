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
	UINT32 m_Height;

	char*  m_Screen;
	HANDLE m_HConsole;
	DWORD  m_DwBytesWritten;

	bool m_Run;
	std::thread m_MainThread;

	Console();

	~Console();

	Console(const Console&) = delete;
	Console& operator=(const Console&) = delete;

	void MainThread();

public:
	static Console& Get();

	void Clear();

	void DrawPoint(float x, float y, char c = '#');

	void DrawLine(float x1, float y1, float x2, float y2);

	void DisplayMessage(const std::string& msg);

	void HeartBeat();

	void Render();
};