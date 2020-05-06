#include <iostream>
#include <list>
#include <string>

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
	std::string name;

public:
	Test(const std::string& name) :
		id(++count),
		name(name)
	{}

	Test(const Test& other) :
		id(++count),
		name(other.name)
	{}

	~Test()
	{}

	void Display() const
	{
		std::cout << "My name is " << name << "-" << id << std::endl;
	}
};

uint32_t Test::count = 0;

int main()
{
	{
		std::list<Test> list;

		SEPARATOR(1);

		list.emplace_back("Joe");
		SEPARATOR(2);
		list.emplace_back("Jack");
		SEPARATOR(3);
		list.emplace_back("William");
		SEPARATOR(4);
		list.emplace_back("Averell");

		for (auto it = list.begin(); it != list.end(); ++it)
			it->Display();
	}

	std::cin.get();
}