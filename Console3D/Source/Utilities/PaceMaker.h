#pragma once

#include <thread>

class PaceMaker
{
private:
	static PaceMaker s_Instance;

	bool m_Run;
	std::thread m_RunThread;

	PaceMaker();
	~PaceMaker();

	PaceMaker(const PaceMaker&) = delete;
	PaceMaker& operator=(const PaceMaker&) = delete;

	void Run();
};