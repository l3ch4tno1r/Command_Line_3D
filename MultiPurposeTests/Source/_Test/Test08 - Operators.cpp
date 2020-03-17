#ifdef TEST08

#include <iostream>

#include "Geometry\Geometry3D\HVector3D.h"

std::ostream& operator<<(std::ostream& stream, const HVector3D& vec)
{
	stream << '(' << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.s << ')';

	return stream;
}

int main()
{
	HVector3D a = { 1.0f, 2.0f, 3.0f };
	HVector3D b = { 4.0f, 5.0f, 6.0f };

	HVector3D c = a + b;

	std::cout << c << std::endl;

	c = a - b;

	std::cout << c << std::endl;

	std::cin.get();
}

#endif // TEST08
