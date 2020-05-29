#include <iostream>

class Test
{
private:
	uint32_t id;

public:
	Test(uint32_t _id) :
		id(_id)
	{
		std::cout << "Test " << id << " constructed" << std::endl;
	}

	~Test()
	{
		std::cout << "Test " << id << " destructed" << std::endl;
	}
};

void func1()
{
	static Test test(1);
}

void func2()
{
	static Test test(2);
}

int main()
{
	Test test(0);

	func2();
	func1();

	std::cin.get();
}