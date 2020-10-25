#pragma once

#include <mutex>
#include <condition_variable>

int main(int argc, char** argv);

class Application
{
public:
	static Application& Get() noexcept;
	void Quit();

private:
	Application()  = default;
	~Application() = default;

	void Run();
	void Wait();

	friend int ::main(int argc, char** argv);

private:
	bool                    m_Run;
	std::mutex              m_RunMut;
	std::condition_variable m_RunCond;
};