#include <iostream>
#include <functional>

void Test(const std::function<bool()>& pred = []() { return true; })
{
	if (pred())
		std::cout << "OK" << std::endl;
	else
		std::cout << "KAPUTT" << std::endl;
}

int main()
{
	int a = 1;

	Test([&]() { return a == 0; });
	Test();

	std::cin.get();
}