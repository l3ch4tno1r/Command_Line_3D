#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>

int Median(std::vector<int>& vec)
{
	int midval = vec[(vec.size() - 1) / 2];

}

int main()
{
	std::srand(unsigned(std::time(0)));

	std::vector<int> vec;

	vec.reserve(15);

	for (int i = 0; i < 15; ++i)
		vec.emplace_back(i);

	std::random_shuffle(vec.begin(), vec.end());

	for (int i : vec)
		std::cout << i << ' ';

	std::cout << std::endl;

	std::cin.get();
}