#pragma once

#include <mutex>
#include <condition_variable>

int main(int argc, char** argv);

class AppTest
{
public:
	static AppTest& Get() noexcept;
	void Quit();

private:
	AppTest()  = default;
	~AppTest() = default;

	void Run();
	void WaitQuit();

	friend int ::main(int argc, char** argv);

private:
	bool                    m_Run;
	std::mutex              m_RunMut;
	std::condition_variable m_RunCond;
};