#include <iostream>
#include <memory>
#include <vector>

#define SEPARATOR(X) std::cout << "------- " << X << " -------" << std::endl

class Test
{
private:
	static uint32_t count;
	uint32_t id;
public:
	Test() :
		id(count++)
	{
		std::cout << "Hello world !" << std::endl;
	}

	void* operator new(size_t size)
	{
		void* ptr = ::new Test();

		std::cout << ptr << " : allocating " << size << " bytes." << std::endl;

		return ptr;
	}

	/*
	void* operator new[](size_t size)
	{
		void* ptr = ::new Test[];

		std::cout << ptr << " : allocating array of " << size << " bytes." << std::endl;

		return ptr;
	}
	*/

	void operator delete(void* ptr, size_t size)
	{
		std::cout << ptr << " : deallocating " << size << " bytes." << std::endl;

		free(ptr);
	}
};

uint32_t Test::count = 0;

int main()
{
	{
		std::vector<Test> test(5);
	}

	std::cin.get();
}