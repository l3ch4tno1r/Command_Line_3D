#include <iostream>

#include "Devices\Console.h"

int main()
{
	HVector3D v1(0, 0, 1);
	HVector3D v2(0, 0, 2);

	const HVector3D& n = HVector3D::Z();
	HVector3D pt(0, 0, 0);

	HVector3D o1(0, 0, 0);
	HVector3D o2(0, 0, 0);

	uint num = Console::ClipEdge(v1, v2, n, pt, o1, o2);

	std::cout << num << std::endl;

	std::cin.get();
}