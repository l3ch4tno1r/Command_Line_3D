#ifndef TRACELOG_H
#define TRACELOG_H

#include <string>
#include <sstream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;

namespace LCNUtilities
{
	typedef pair<string, string> FileMsgPair;

	//////////////////
	//-- TraceLog --//
	//////////////////

	class TraceLog
	{
	private:
		string logfilepath;

		bool done;

		queue<FileMsgPair> logqueue;
		mutex              logmutex;
		condition_variable logcondition;
		thread             logthread;

		TraceLog();
		TraceLog(const TraceLog&) = delete;

		TraceLog& operator=(const TraceLog&) = delete;

		~TraceLog();


		inline FileMsgPair GetFrontPair();

		//-- Log to Console --//
		static inline void WriteToConsole(const string &msg);

		//-- Log to File --//
		static inline void WriteToFile(const string &logfilepath, const string &msg);

		void LogLoop();

	public:

		static TraceLog& Logger() noexcept;

		void AddToQueue(const string &logfilepath, const string &msg);
	};

	/////////////
	//-- Log --//
	/////////////

	class Log
	{
	protected:
		stringstream ssmsg;
		string logfilepath;

	public:
		Log();

		Log(const string &path);

		~Log();

		template<class C>
		Log& operator<<(const C &input);

		Log& operator<<(const exception &e);
	};

	template<class C>
	inline Log& Log::operator<<(const C &input)
	{
		ssmsg << input;

		return *this;
	}
}

#endif
