#include <iostream>
#include <unordered_set>

#define SEPARATOR(X) std::cout << "------- " << X << " -------" << std::endl

void* operator new(size_t size)
{
	void* ptr = malloc(size);

	std::cout << ptr << " : Allocating " << size << " bytes" << std::endl;

	return ptr;
}

void operator delete(void* ptr, size_t size)
{
	std::cout << ptr << " : Deallocating " << size << " bytes" << std::endl;

	free(ptr);
}

class Hash
{
public:
	size_t operator()(const char& c) const
	{
		size_t result = std::hash<char>()(c);

		std::cout << "Hashing " << (uint16_t)c << " = " << result << std::endl;

		return result;
	}
};

int main()
{
	{
		std::unordered_set<char, Hash> set;

		SEPARATOR("Begin");

		for (uint8_t c = 0; c != 255; c++)
		{
			SEPARATOR((uint16_t)c);

			set.emplace(c);
		}

		SEPARATOR("End");
	}

	std::cin.get();
}