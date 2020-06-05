#include <iostream>
#include <queue>

#include "Utilities/Instrumentor.h"

#define SEPARATOR(X) std::cout << "------- " << X << " -------" << std::endl

int numelement;

/*
void* operator new(size_t size)
{
	std::cout << "Allocating " << size << " bytes." << std::endl;

	return malloc(size);
}
*/

void NaiveQueue()
{
	std::queue<int> _queue;

	for (int i = 0; i < numelement; ++i)
		_queue.emplace(i);

	while (!_queue.empty())
		_queue.pop();
}

class ReusableQueue
{
private:
	std::queue<int> m_Queue;

public:
	void operator()()
	{
		for (int i = 0; i < numelement; ++i)
			m_Queue.emplace(i);

		while (!m_Queue.empty())
			m_Queue.pop();
	}
};

int main()
{
	Instrumentor::Get().BeginSession("Test");

	uint32_t numtest;

	std::cout << "Nombre de Test :" << std::endl;
	std::cin >> numtest; std::cin.ignore();

	std::cout << "Nombre d'elements :" << std::endl;
	std::cin >> numelement; std::cin.ignore();

	{
		PROFILE_SCOPE("Naive Queue");

		for(uint32_t i = 0; i < numtest; ++i)
			NaiveQueue();
	}

	//SEPARATOR(1);

	{
		PROFILE_SCOPE("Reusable Queue");

		ReusableQueue rqueue;

		for (uint32_t i = 0; i < numtest; ++i)
			rqueue();
	}

	std::cout << "Done" << std::endl;

	std::cin.get();
}