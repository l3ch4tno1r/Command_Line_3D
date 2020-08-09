#include <iostream>
#include <type_traits>

template<typename T>
struct is_pointer : std::false_type
{
	typedef T Type;
};

template<typename T>
struct is_pointer<T*> : std::true_type
{
	typedef T Type;
};

int main()
{
	typedef is_pointer<double>::Type d1;
	typedef is_pointer<double*>::Type d2;
	typedef is_pointer<double**>::Type d3;
	typedef is_pointer<const double*>::Type d4;
	typedef is_pointer<double* const>::Type d5;

	d4 a = 0.0;

	std::cout << is_pointer<int>::value << std::endl;
	std::cout << is_pointer<int*>::value << std::endl;

	std::cin.get();
}