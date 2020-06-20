#include <iostream>
#include <exception>

void func() throw(std::exception)
{
	std::cout << "Hello world !" << std::endl;

	throw std::bad_alloc();
}

int main()
{
	std::set_unexpected([]
	{
		std::cerr << "Unexpected !" << std::endl;
		std::abort();
	});

	func();

	try
	{
		func();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	std::cin.get();
}