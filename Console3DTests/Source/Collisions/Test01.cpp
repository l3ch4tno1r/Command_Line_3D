#include <iostream>

#include <LCN_Collisions/Source/Shapes/Line.h>

int main()
{
	LCN::Line2Df line(
		{ 1, 2, 1 },
		{ 1, 1, 0 });

	std::cout << line.Origin() << std::endl;
	std::cout << line.Direction() << std::endl;

	std::cin.get();
}