#ifdef TEST11

#include <iostream>

int main()
{
	int tab[3] = { 4, 5, 6 };

	for (int i : tab)
		std::cout << i << std::endl;

	std::cin.get();
}

#endif // TEST11
