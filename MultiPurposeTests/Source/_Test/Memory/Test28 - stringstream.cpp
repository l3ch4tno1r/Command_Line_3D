#include <iostream>
#include <sstream>

static uint32_t numallocation = 0;

#define SEPARATOR(X) std::cout << "------- " << X << " -------" << std::endl

void* operator new(size_t size)
{
	++numallocation;

	std::cout << "Allocated : " << size << " bytes." << std::endl;

	return malloc(size);
}

int main()
{
	std::stringstream sstr;

	SEPARATOR(1);

	for (int i = 0; i < 100; ++i)
		sstr << i;

	SEPARATOR(2);

	sstr.clear();

	SEPARATOR(3);

	sstr << "Hello world !";

	SEPARATOR(4);

	std::cout << "Number of allocations : " << numallocation << std::endl;

	std::cin.get();
}