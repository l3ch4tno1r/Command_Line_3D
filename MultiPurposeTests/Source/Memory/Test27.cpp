#include <iostream>

void* operator new(size_t size, const char* file, int line)
{
	std::cout << "Allocating " << size << " bytes at " << file << " line " << line << std::endl;

	return malloc(size);
}

void operator delete(void* ptr, size_t size)
{
	std::cout << "Deallocating " << size << " bytes" << std::endl;

	free(ptr);
}

#define new new(__FILE__, __LINE__)

int main()
{
	int* ptr = new int(0);

	delete ptr;

	std::cin.get();
}