#include <iostream>
#include <memory>
#include <vector>
#include <limits>

#define SEPARATOR(X) std::cout << "------- " << X << " -------" << std::endl

template<typename T>
struct CustomAllocator
{
	using value_type = T;

	using pointer = T* ;
	using const_pointer = const T*;

	using void_pointer = void*;
	using const_void_pointer = const void*;

	using size_type = size_t;

	using difference_type = std::ptrdiff_t;

	CustomAllocator() = default;
	~CustomAllocator() = default;

	T* allocate(size_type numobj)
	{
		std::cout << "Allocating " << numobj << " elements." << std::endl;

		return static_cast<pointer>(operator new(sizeof(T) * numobj));
	}

	void deallocate(pointer p, size_type numobj)
	{
		std::cout << "Deallocating " << numobj << " elements." << std::endl;

		operator delete(p);
	}

	size_type max_size() const
	{
		return std::numeric_limits<size_type>::max();
	}
};

int main()
{
	std::vector<int, CustomAllocator<int>> vec;

	for (int i = 0; i < 10; i++)
	{
		SEPARATOR(i);
		vec.push_back(i);
	}

	SEPARATOR("Loop");

	for (const int& a : vec)
		std::cout << a << std::endl;

	std::cin.get();
}