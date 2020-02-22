#ifdef TEST05

#include <iostream>

#include "Utilities\PaceMaker.h"

int main()
{
	std::mutex mut;

	for (int i = 0; i < 10; i++)
	{
		std::unique_lock<std::mutex> lock(mut);

		PaceMaker::Get().Wait(lock);

		std::cout << "i = " << i << std::endl;
	}

	std::cin.get();
}

#endif // TEST05
