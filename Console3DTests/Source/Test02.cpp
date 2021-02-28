#include <iostream>

class Test
{
public:
	Test()
	{
		throw std::exception("Test");
	}
};

int main()
{
	try
	{
		Test();
	}
	catch (const std::exception& e)
	{
		std::cerr << "Exception : " << e.what() << std::endl;
	}

	std::cin.get();
}