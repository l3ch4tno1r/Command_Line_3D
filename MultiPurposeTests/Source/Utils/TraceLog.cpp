#include <fstream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>

#include "TraceLog.h"

#define TEST

//////////////////
//-- TraceLog --//
//////////////////

//-- Constructors and Destructors --//

namespace LCNUtilities
{
	TraceLog::TraceLog() :
		m_Done(false)
	{
		auto now = std::chrono::system_clock::now();
		auto in_time_t = std::chrono::system_clock::to_time_t(now);

		std::stringstream ss;
		//ss << put_time(std::localtime(&in_time_t), "%Y%m%d%Hh%Mm%Ss");

		m_LogFilePath = "APP" + ss.str() + ".log";

		m_LogThread = std::thread(&TraceLog::MainThread, this);
	}

	TraceLog::~TraceLog()
	{
		m_Done = true;

		m_LogCondition.notify_one();

		m_LogThread.join();
	}

	//-- Accessor --//

	TraceLog& TraceLog::Logger() noexcept
	{
		static TraceLog logger;
		return logger;
	}

	//-- Log to Console --//

	inline void TraceLog::WriteToConsole(const std::string &msg)
	{
		std::cout << msg + '\n';

		std::cout.flush();
	}

	//-- Log to File --//
	inline void TraceLog::WriteToFile(const std::string &m_LogFilePath, const std::string &msg)
	{
		std::ofstream logfile(m_LogFilePath, std::ios::out | std::ios::app);

		if (logfile.is_open())
		{
			logfile << msg + '\n';

			logfile.close();
		}
	}

	void TraceLog::AddToQueue(const std::string &m_LogFilePath, const std::string &msg)
	{
		std::unique_lock<std::mutex> lock(m_LogMutex);

		m_LogQueue.push(FileMsgPair(m_LogFilePath, msg));

		m_LogCondition.notify_one();
	}

	inline FileMsgPair TraceLog::GetFrontPair()
	{
		FileMsgPair filemsg;
		std::unique_lock<std::mutex> lock(m_LogMutex);

		while (m_LogQueue.empty() && !m_Done)
			m_LogCondition.wait(lock);

		if (!m_LogQueue.empty())
		{
			filemsg = m_LogQueue.front();

			m_LogQueue.pop();
		}

		return filemsg;
	}

	void TraceLog::MainThread()
	{
		while (!m_Done || !m_LogQueue.empty())
		{
			FileMsgPair filemsg = GetFrontPair();

			if (filemsg.first.empty())
				WriteToConsole(filemsg.second);
			else
				WriteToFile(filemsg.first, filemsg.second);
		}
	}

	//-- Instanciation --//

	//TraceLog TraceLog::logger;

	/////////////
	//-- Log --//
	/////////////

	Log::Log() :
		m_LogFilePath()
	{}

	Log::Log(const std::string &path) :
		m_LogFilePath(path)
	{}

	Log::~Log()
	{
		TraceLog::Logger().AddToQueue(m_LogFilePath, m_ssMsg.str());
	}

	Log& Log::operator<<(const std::exception& e)
	{
		m_ssMsg << "Exception : " << e.what();

		return *this;
	}
}