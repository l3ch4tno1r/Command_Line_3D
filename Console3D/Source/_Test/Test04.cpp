#ifdef TEST04

#include <iostream>

#include "Matrix3D\Vector3D.h"

std::ostream& operator<<(std::ostream& stream, const Vector3D& vec)
{
	stream << '(' << vec.x << ", " << vec.y << ", " << vec.z << ')' << std::endl;

	return stream;
}

int main()
{
	Vector3D vec1;
	Vector3D vec2(1.0f, 2.0f, 3.0f);
	Vector3D vec3 = vec1 ^ vec2;

	std::cout << vec1 << std::endl;
	std::cout << vec2 << std::endl;
	std::cout << vec3 << std::endl;

	std::cin.get();
}

#endif // TEST04
