#include <iostream>
#include <future>

int TestThread(std::future<int>& future)
{
	try
	{
		int a = future.get();

		return a + 10;
	}
	catch (const std::exception& e)
	{
		throw e;
	}
}

int main()
{
	try
	{
		std::promise<int> p;
		std::future<int> f0 = p.get_future();

		std::future<int> f1 = std::async(std::launch::async, TestThread, std::ref(f0));

		p.set_value(5);
		//p.set_exception(std::make_exception_ptr(std::runtime_error("WOUPS ^^'")));
		
		std::cout << f1.get() << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	std::cin.get();
}