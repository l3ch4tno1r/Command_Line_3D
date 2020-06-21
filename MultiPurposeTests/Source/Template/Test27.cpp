#include <iostream>
#include <type_traits>

class Test
{
public:
	Test() = default;

	Test(const Test&)
	{
		std::cout << "Copied" << std::endl;
	}

	Test(Test&&)
	{
		std::cout << "Moved" << std::endl;
	}
};

int main()
{
	std::cout << std::boolalpha;

	std::cout << std::is_same<Test, std::remove_reference<Test  >::type>() << std::endl;
	std::cout << std::is_same<Test, std::remove_reference<Test& >::type>() << std::endl;
	std::cout << std::is_same<Test, std::remove_reference<Test&&>::type>() << std::endl;

	Test test1;

	Test test2 = std::move(test1);

	std::cin.get();
}