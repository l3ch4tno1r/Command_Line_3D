#ifdef TEST09

#include <iostream>

int main()
{
	enum Test
	{
		Behind = -1,
		Center =  1,
		Top    =  2,
		Bottom =  4,
		Left   =  8,
		Right  =  16
	};

	short a = 0;

	a |= Test::Center;

	std::cout << a << std::endl;

	a |= Test::Left;

	std::cout << a << std::endl;

	a = Test::Top | Test::Right;

	std::cout << a << std::endl;

	std::cin.get();
}

#endif // TEST09
