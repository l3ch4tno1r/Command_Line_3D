#ifndef TRACELOG_H
#define TRACELOG_H

#include <string>
#include <sstream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

namespace LCNUtilities
{
	typedef std::pair<std::string, std::string> FileMsgPair;

	//////////////////
	//-- TraceLog --//
	//////////////////

	class TraceLog
	{
	private:
		std::string m_LogFilePath;

		std::atomic_bool m_Done;

		std::queue<FileMsgPair> m_LogQueue;
		std::mutex              m_LogMutex;
		std::condition_variable m_LogCondition;
		std::thread             m_LogThread;

		TraceLog();

		TraceLog(const TraceLog&) = delete;
		TraceLog& operator=(const TraceLog&) = delete;

		~TraceLog();

		inline FileMsgPair GetFrontPair();

		//-- Log to Console --//
		static inline void WriteToConsole(const std::string &msg);

		//-- Log to File --//
		static inline void WriteToFile(const std::string &m_LogFilePath, const std::string &msg);

		void MainThread();

	public:

		static TraceLog& Logger() noexcept;

		void AddToQueue(const std::string &m_LogFilePath, const std::string &msg);
	};

	/////////////
	//-- Log --//
	/////////////

	class Log
	{
	protected:
		std::stringstream m_ssMsg;
		std::string m_LogFilePath;

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
		m_ssMsg << input;

		return *this;
	}
}

#endif
