#include <iostream>
#include <fstream>
#include <string>
#include <functional>
#include <array>
#include <vector>

#define SEPARATOR(X) std::cout << "------- " << #X << " -------" << std::endl

void* operator new(size_t size)
{
	std::cout << "Allocating " << size << " bytes" << std::endl;

	return malloc(size);
}

void operator delete(void* ptr, size_t size)
{
	std::cout << "Deallocating " << size << " bytes" << std::endl;

	free(ptr);
}

int main()
{
	try
	{
		std::hash<std::string> hashfct;

		SEPARATOR(1);

		std::array<std::vector<unsigned int>, 4> collisions;

		SEPARATOR(2);

		size_t size = 8;

		for (unsigned int i = 0; i < collisions.size(); ++i)
			collisions[i] = std::vector<unsigned int>(size *= 2, 0);

		SEPARATOR(3);

		std::ifstream dictionarystream("Ressources/liste_francais.txt", std::ios::in);

		if (!dictionarystream)
			throw std::exception("File not found !");

		std::string line;

		SEPARATOR(4);

		while (std::getline(dictionarystream, line))
		{
			size_t hash = hashfct(line);

			for (auto& vec : collisions)
				vec[hash % vec.size()]++;
		}

		SEPARATOR(5);
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	std::cout << "Done !" << std::endl;

	std::cin.get();
}