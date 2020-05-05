#include <iostream>
#include <list>

#define SEPARATOR(X) std::cout << "------- " << X << " -------" << std::endl

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

class Test
{
private:
	static uint32_t count;
	uint32_t id;

public:
	Test() : id(++count)
	{
		std::cout << "Hello world ! " << id << std::endl;
	}

	Test(const Test&) : id(++count)
	{
		std::cout << "Hello world copy ! " << id << std::endl;
	}

	~Test()
	{
		std::cout << "Goodbye world ! " << id << std::endl;
	}

	void Display() const
	{
		std::cout << "Display ! " << id << std::endl;
	}
};

uint32_t Test::count = 0;

int main()
{
	{
		std::list<Test> list;

		SEPARATOR(1);

		list.emplace_back();
		SEPARATOR(2);
		list.emplace_back();
		SEPARATOR(3);
		list.emplace_back();
		SEPARATOR(4);
		list.emplace_back();
	}

	std::cin.get();
}