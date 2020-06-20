#include <iostream>
#include <string>
#include <type_traits>

#include "Utilities/ErrorHandling.h"

class Test
{
public:
	Test() = default;

	Test(const Test&)
	{
		std::cerr << "Copied" << std::endl;
	}
};

std::ostream& operator<<(std::ostream& stream, const Test&)
{
	stream << "Class Test";

	return stream;
}

template<typename T>
void Print(const T& a)
{
	bool test = std::is_same<T, const char*>::value;

	BREACKIF(test);

	std::cout << a;
}

template<typename T, typename ... Args>
void Print(const T& a, const Args&... args)
{
	Print(a);
	Print(args...);
}

int main()
{
	Test test;

	const char* str = "raw string";

	Print(1, test, str);

	std::move(test);

	std::cin.get();
}