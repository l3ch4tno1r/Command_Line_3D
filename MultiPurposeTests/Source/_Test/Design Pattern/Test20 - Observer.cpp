#include <iostream>
#include <list>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

using namespace std::literals::chrono_literals;

class ISubject;

class IObserver
{
private:
	ISubject* m_SubjectPtr;

	static uint32_t count;

protected:
	uint32_t id;

public:
	IObserver() :
		m_SubjectPtr(nullptr),
		id(++count)
	{}

	virtual ~IObserver();

	virtual void Update() = 0;

	void AttachSubject(ISubject* _subject)
	{
		m_SubjectPtr = _subject;
	}

	void DetachSubject()
	{
		m_SubjectPtr = nullptr;
	}
};

uint32_t IObserver::count = 0;

class ISubject
{
private:
	std::list<IObserver*> m_ObsList;

public:
	void AddObserver(IObserver* obs)
	{
		obs->AttachSubject(this);

		m_ObsList.push_back(obs);
	}

	void RemoveObserver(IObserver* obs)
	{
		auto it = std::find(m_ObsList.begin(), m_ObsList.end(), obs);

		(*it)->DetachSubject();

		m_ObsList.erase(it);
	}

	void Notify()
	{
		for (auto& obs : m_ObsList)
			obs->Update();
	}
};

IObserver::~IObserver()
{
	if (m_SubjectPtr != nullptr)
		m_SubjectPtr->RemoveObserver(this);
}

////////////////////////
//-- Implementation --//
////////////////////////

class PaceMaker : public ISubject
{
private:
	std::atomic_bool m_Run;
	std::thread m_MainThread;

	void MainThread()
	{
		while (m_Run)
		{
			std::cout << "Heartbeat !" << std::endl;

			Notify();

			std::this_thread::sleep_for(1s);
		}
	}

public:
	PaceMaker() :
		m_Run(true),
		m_MainThread(&PaceMaker::MainThread, this)
	{}

	~PaceMaker()
	{
		m_Run = false;

		m_MainThread.join();
	}
};

class Worker : public IObserver
{
private:
	std::mutex              m_Mutex;
	std::condition_variable m_Condition;
	std::atomic_bool        m_Run, m_Notified;

	std::thread m_MainThread;

	void Wait()
	{
		std::unique_lock<std::mutex> lock(m_Mutex);

		while (!m_Notified)
			m_Condition.wait(lock);

		m_Notified = false;
	}

	void MainThread()
	{
		while (m_Run)
		{
			Wait();

			std::cout << "Thread #" << std::this_thread::get_id() << " working..." << std::endl;
		}
	}

public:
	Worker() :
		m_Run(true),
		m_Notified(false),
		m_MainThread(&Worker::MainThread, this)
	{}

	~Worker()
	{
		m_Run = false;

		m_MainThread.join();
	}

	void Update() override
	{
		std::unique_lock<std::mutex> lock(m_Mutex);

		m_Notified = true;

		m_Condition.notify_one();
	}
};

int main()
{
	PaceMaker pacemaker;

	Worker w1;
	Worker w2;
	Worker w3;

	pacemaker.AddObserver(&w1);
	pacemaker.AddObserver(&w2);
	pacemaker.AddObserver(&w3);

	std::cin.get();
}

