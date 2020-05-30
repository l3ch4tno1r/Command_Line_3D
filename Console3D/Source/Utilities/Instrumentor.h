//
// Basic instrumentation profiler by Cherno

// Usage: include this header file somewhere in your code (eg. precompiled header), and then use like:
//
// Instrumentor::Get().BeginSession("Session Name");        // Begin session 
// {
//     InstrumentationTimer timer("Profiled Scope Name");   // Place code like this in scopes you'd like to include in profiling
//     // Code
// }
// Instrumentor::Get().EndSession();                        // End Session
//
// You will probably want to macro-fy this, to switch on/off easily and use things like __FUNCSIG__ for the profile name.

#pragma once

#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <queue>

#define PROFILING 1

#if PROFILING
#define PROFILE_SCOPE(name) InstrumentationTimer time ## __LINE__(name)
#define PROFILE_FUNC() PROFILE_SCOPE(__FUNCSIG__)
#else
#define PROFILE_SCOPE(name)
#define PROFILE_FUNC()
#endif

struct ProfileResult
{
	std::string Name;
	long long   Start, End;
	uint32_t    ThreadID;
};

class Instrumentor
{
private:
	std::string     m_SessionName;
	std::atomic_int m_ProfileCount;

	std::queue<ProfileResult> m_ProfileQueue;
	std::mutex                m_Mutex;
	std::condition_variable   m_Condition;

	std::atomic_bool m_SessionRunning;
	std::thread      m_MainThread;

	Instrumentor() :
		m_SessionName("None"),
		m_ProfileCount(0)
	{}

	~Instrumentor()
	{
		EndSession();
	}

	bool GetFrontProfile(ProfileResult& result)
	{
		std::unique_lock<std::mutex> lock(m_Mutex);

		while (m_ProfileQueue.empty() && m_SessionRunning)
			m_Condition.wait(lock);

		if (!m_ProfileQueue.empty())
		{
			result = m_ProfileQueue.front();

			m_ProfileQueue.pop();
		}

		return m_SessionRunning;
	}

	void MainThread(const std::string& filepath)
	{
		std::ofstream outputstream(filepath);

		// Write header
		outputstream << "{\"otherData\": {},\"traceEvents\":[";
		outputstream.flush();

		ProfileResult result;

		while(GetFrontProfile(result))
		{
			if (m_ProfileCount++ > 0)
				outputstream << ",\n";

			std::string name = result.Name;
			std::replace(name.begin(), name.end(), '"', '\'');

			outputstream << "\t{\n";
			outputstream << "\t\t\"cat\" :\"function\",\n";
			outputstream << "\t\t\"dur\" :" << (result.End - result.Start) << ",\n";
			outputstream << "\t\t\"name\":\"" << name << "\",\n";
			outputstream << "\t\t\"ph\"  :\"X\",\n";
			outputstream << "\t\t\"pid\" :0,\n";
			outputstream << "\t\t\"tid\" :" << result.ThreadID << ",\n";
			outputstream << "\t\t\"ts\"  :" << result.Start << "\n";
			outputstream << "\t}";

			outputstream.flush();
		}

		// Write footer
		outputstream << "]}";
		outputstream.flush();

		outputstream.close();
	}

public:
	void BeginSession(const std::string& name, const std::string& filepath = "results.json")
	{
		m_SessionName = name;

		m_SessionRunning = true;

		if(!m_MainThread.joinable())
			m_MainThread = std::thread(&Instrumentor::MainThread, this, std::move(filepath));
	}

	void EndSession()
	{
		m_ProfileCount = 0;

		m_SessionRunning = false;

		m_Condition.notify_one();

		if (m_MainThread.joinable())
			m_MainThread.join();
	}

	void WriteProfile(const ProfileResult& result)
	{
		std::lock_guard<std::mutex> lock(m_Mutex);

		m_ProfileQueue.push(result);

		m_Condition.notify_one();
	}

	static Instrumentor& Get()
	{
		static Instrumentor instance;
		return instance;
	}
};

class InstrumentationTimer
{
public:
	InstrumentationTimer(const char* name)
		: m_Name(name), m_Stopped(false)
	{
		m_StartTimepoint = std::chrono::high_resolution_clock::now();
	}

	~InstrumentationTimer()
	{
		if (!m_Stopped)
			Stop();
	}

	void Stop()
	{
		auto endTimepoint = std::chrono::high_resolution_clock::now();

		long long start = std::chrono::time_point_cast<std::chrono::nanoseconds>(m_StartTimepoint).time_since_epoch().count();
		long long end   = std::chrono::time_point_cast<std::chrono::nanoseconds>(endTimepoint).time_since_epoch().count();

		uint32_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());

		Instrumentor::Get().WriteProfile({ m_Name, start, end, threadID });

		m_Stopped = true;
	}

private:
	const char* m_Name;
	std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
	bool m_Stopped;
};