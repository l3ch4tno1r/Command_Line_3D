#include <iostream>
#include <string>

void* operator new(size_t size)
{
	std::cout << "Allocating " << size << " bytes." << std::endl;

	return std::malloc(size);
}

int main()
{
	std::string str1 = "Hello world ! My name is Matt !";

	//const std::string& ref = str1;

	std::string str2 = std::move(str1);

	std::cout << str1 << std::endl;
	std::cout << str2 << std::endl;

	std::cin.get();
}