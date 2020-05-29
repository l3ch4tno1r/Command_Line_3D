#include <iostream>
#include <string>

void func1();

class Test
{
private:
	std::string name;

public:
	Test(const char* str) :
		name(str)
	{
		func1();
		std::cout << "Test " << name << " constructed" << std::endl;
	}

	~Test()
	{
		std::cout << "Test " << name << " destructed" << std::endl;
	}
};

void func1()
{
	static Test test("func1");
}

void func2()
{
	static Test test("func2");
}

Test global("global");

int main()
{
	Test test("main");

	func1();
	func2();

	std::cin.get();
}