#include <iostream>

#include "Devices\Console.h"

/*
void Test01()
{
	HVector3D v1(0, 0, 1);
	HVector3D v2(0, 0, 2);

	const HVector3D& n = HVector3D::Z();

	HVector3D pts[] = {
		HVector3D(0, 0, 0),
		HVector3D(0, 0, 1.5),
		HVector3D(0, 0, 3)
	};

	HVector3D o1(0, 0, 0);
	HVector3D o2(0, 0, 0);

	for (const HVector3D& pt : pts)
	{
		uint num = Console::ClipEdge(v1, v2, n, pt, o1, o2);

		std::cout << num << std::endl;
	}
}

*/

void Test02()
{
	Console::Triangle triangle = {
		HVector3D(0, 0, 1),
		HVector3D(1, 0, 2),
		HVector3D(0, 0, 3)
	};

	const HVector3D& n = HVector3D::Z();

	HVector3D pts[] = {
		HVector3D(0, 0, 0),
		HVector3D(0, 0, 1.5),
		HVector3D(0, 0, 2.5),
		HVector3D(0, 0, 3.5),
	};

	Console::Triangle o1 = {
		HVector3D(0, 0, 0),
		HVector3D(0, 0, 0),
		HVector3D(0, 0, 0)
	};
	
	Console::Triangle o2 = {
		HVector3D(0, 0, 0),
		HVector3D(0, 0, 0),
		HVector3D(0, 0, 0)
	};

	for (const HVector3D& pt : pts)
	{
		uint num = Console::ClipTriangle(triangle, n, pt, o1, o2);

		std::cout << num << std::endl;
	}
}

int main()
{
	Test02();

	std::cin.get();
}