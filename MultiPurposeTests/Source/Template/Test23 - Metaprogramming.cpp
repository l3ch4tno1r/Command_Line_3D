#include <iostream>

template<uint32_t N>
struct Factorielle
{
	enum { valeur = N * Factorielle<N - 1>::valeur };
};

template<>
struct Factorielle<0>
{
	enum { valeur = 1 };
};

int main()
{
	std::cout << "Factorielle 5 = " << Factorielle<5>::valeur << std::endl;

	std::cin.get();
}