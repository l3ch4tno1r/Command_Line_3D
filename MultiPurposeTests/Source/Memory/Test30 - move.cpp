#include <iostream>
#include <string>

void* operator new(size_t size)
{
	std::cout << "Allocating " << size << " bytes." << std::endl;

	return std::malloc(size);
}

class Test
{
public:
	Test()
	{
		std::cout << "Default ctor" << std::endl;
	}

	Test(const Test&)
	{
		std::cout << "Copy ctor" << std::endl;
	}

	Test(Test&&)
	{
		std::cout << "Move ctor" << std::endl;
	}

	~Test()
	{
		std::cout << "dtor" << std::endl;
	}
};

int main()
{
	{
		Test test1;

		const Test& ref = test1;

		Test test2 = std::move(ref);
	}

	std::cin.get();
}