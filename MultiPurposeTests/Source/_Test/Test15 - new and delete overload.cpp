#include <iostream>
#include <fstream>
#include <queue>
#include <stack>

void* operator new(size_t size)
{
	std::cout << "Allocating " << size << " bytes" << std::endl;

	return malloc(size);
}

void operator delete(void* ptr, size_t size)
{
	std::cout << "Deallocating " << size << " bytes" << std::endl;

	free(ptr);
}

#define LOGADDRESS(X) stream << (int)&(X) << std::endl

int main()
{
	{
		std::queue<int> queue;
		std::ofstream   stream("memory.csv", std::ios::out);

		std::cout << "-- Pushing #1 --" << std::endl;

		for (uint32_t i = 0; i < 100; i++)
		{
			std::cout << "Pushing " << i << std::endl;
			queue.push(i);
			LOGADDRESS(queue.front());
		}

		std::cout << "-- Poping #1 --" << std::endl;

		while (queue.front() != 50)
		{
			std::cout << "Poping " << queue.front() << std::endl;
			LOGADDRESS(queue.front());
			queue.pop();
		}

		std::cout << "-- Pushing #2 --" << std::endl;

		for (uint32_t i = 0; i < 100; i++)
		{
			std::cout << "Pushing " << i << std::endl;
			queue.push(i);
			LOGADDRESS(queue.front());
		}

		std::cout << "-- Poping #2 --" << std::endl;

		while (!queue.empty())
		{
			std::cout << "Poping " << queue.front() << std::endl;
			LOGADDRESS(queue.front());
			queue.pop();
		}

		std::cout << "----------------" << std::endl;
	}

	std::cin.get();
}