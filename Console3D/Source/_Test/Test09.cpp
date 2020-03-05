#ifdef TEST09

#include <iostream>

#include "Utilities\Utils.h"

int main()
{
	std::cout << sign(2) << std::endl;
	std::cout << sign(-3) << std::endl;
	std::cout << sign(2.4f) << std::endl;
	std::cout << sign(0) << std::endl;

	std::cin.get();
}

#endif // TEST09
