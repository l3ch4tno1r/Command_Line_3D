#include <iostream>

#include "Source\Utils\ErrorHandling.h"

struct Vector2D
{
	float x;
	float y;

	float& operator[](std::size_t i)
	{
		return *((float*)this + i);
	}
};

int main()
{
	Vector2D vec = { 1, 2 };

	ASSERT(vec.x == vec[0]);
	ASSERT(vec.y == vec[1]);

	std::cout << vec.x << ", " << vec[0] << std::endl;
	std::cout << vec.y << ", " << vec[1] << std::endl;

	std::cin.get();
}