#ifndef TRACELOG_H
#define TRACELOG_H

#include <string>
#include <sstream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

namespace LCNUtilities
{
	typedef std::pair<std::string, std::string> FileMsgPair;

	//////////////////
	//-- TraceLog --//
	//////////////////

	class TraceLog
	{
	private:
		std::string logfilepath;

		bool done;

		std::queue<FileMsgPair> logqueue;
		std::mutex              logmutex;
		std::condition_variable logcondition;
		std::thread             logthread;

		TraceLog();

		TraceLog(const TraceLog&) = delete;
		TraceLog& operator=(const TraceLog&) = delete;

		~TraceLog();

		inline FileMsgPair GetFrontPair();

		//-- Log to Console --//
		static inline void WriteToConsole(const std::string &msg);

		//-- Log to File --//
		static inline void WriteToFile(const std::string &logfilepath, const std::string &msg);

		void LogLoop();

	public:

		static TraceLog& Logger() noexcept;

		void AddToQueue(const std::string &logfilepath, const std::string &msg);
	};

	/////////////
	//-- Log --//
	/////////////

	class Log
	{
	protected:
		std::stringstream ssmsg;
		std::string logfilepath;

	public:
		Log();

		Log(const std::string &path);

		~Log();

		template<class C>
		Log& operator<<(const C &input);

		Log& operator<<(const std::exception &e);
	};

	template<class C>
	inline Log& Log::operator<<(const C &input)
	{
		ssmsg << input;

		return *this;
	}
}

#endif
