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
	int sep = 0;

	std::ostringstream sstr;

	SEPARATOR(++sep); // 1

	for (int i = 0; i < 100; ++i)
		sstr << i;

	SEPARATOR(++sep); // 2

	sstr.clear(); sstr.seekp(0);

	SEPARATOR(++sep); // 3
	std::cout << sstr.str() << std::endl;

	SEPARATOR(++sep);

	sstr.clear(); sstr.seekp(0);
	sstr << "Hello world !" << std::ends;
	std::cout << sstr.str() << std::endl;

	SEPARATOR(++sep);

	std::cout << "Number of allocations : " << numallocation << std::endl;

	std::cin.get();
}