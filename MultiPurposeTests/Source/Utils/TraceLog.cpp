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
		done(false),
		logthread(&TraceLog::LogLoop, this)
	{
		auto now = std::chrono::system_clock::now();
		auto in_time_t = std::chrono::system_clock::to_time_t(now);

		std::stringstream ss;
		//ss << put_time(std::localtime(&in_time_t), "%Y%m%d%Hh%Mm%Ss");

		logfilepath = "APP" + ss.str() + ".log";
	}

	TraceLog::~TraceLog()
	{
		done = true;

		logcondition.notify_all();

		logthread.join();
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
	inline void TraceLog::WriteToFile(const std::string &logfilepath, const std::string &msg)
	{
		std::ofstream logfile(logfilepath, std::ios::out | std::ios::app);

		if (logfile.is_open())
		{
			logfile << msg + '\n';

			logfile.close();
		}
	}

	void TraceLog::AddToQueue(const std::string &logfilepath, const std::string &msg)
	{
		std::unique_lock<std::mutex> lock(logmutex);

		logqueue.push(FileMsgPair(logfilepath, msg));

		logcondition.notify_one();
	}

	inline FileMsgPair TraceLog::GetFrontPair()
	{
		FileMsgPair filemsg;
		std::unique_lock<std::mutex> lock(logmutex);

		logcondition.wait(lock, [&]
		{
			return (!logqueue.empty() || done); 
		});

		if (!logqueue.empty())
		{
			filemsg = logqueue.front();

			logqueue.pop();
		}

		return filemsg;
	}

	void TraceLog::LogLoop()
	{
		while (!done || !logqueue.empty())
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
		logfilepath()
	{}

	Log::Log(const std::string &path) :
		logfilepath(path)
	{}

	Log::~Log()
	{
		TraceLog::Logger().AddToQueue(logfilepath, ssmsg.str());
	}

	Log& Log::operator<<(const std::exception& e)
	{
		ssmsg << "Exception : " << e.what();

		return *this;
	}
}