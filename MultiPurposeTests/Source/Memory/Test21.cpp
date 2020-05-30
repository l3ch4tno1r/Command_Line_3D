#include <iostream>
#include <string>

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

int main()
{
	{
		using namespace std::literals::string_literals;

		std::string str;

		std::cout << "----------------" << std::endl;
		str = "Hello world !" + " How are you ?"s;
		std::cout << "----------------" << std::endl;
	}

	std::cin.get();
}