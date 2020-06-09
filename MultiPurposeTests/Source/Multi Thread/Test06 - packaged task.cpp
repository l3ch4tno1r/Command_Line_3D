#include <iostream>
#include <future>

int Add(int a, int b)
{
	return a + b;
}

int Sub(int a, int b)
{
	return a - b;
}

namespace ph = std::placeholders;

int main()
{
	std::packaged_task<int(int, int)> task(std::bind(Sub, ph::_2, ph::_1));

	task(9, 3);

	std::cout << task.get_future().get() << std::endl;

	std::cin.get();
}