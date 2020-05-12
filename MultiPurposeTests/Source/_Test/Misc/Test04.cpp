#ifdef TEST04

#include <iostream>

#include "Geometry\Geometry2D\HVector2D.h"
#include "Geometry\Geometry2D\Transform2D.h"
#include "Geometry\Geometry3D\HVector3D.h"
//#include "Geometry\Geometry3D\Transform3D.h"

std::ostream& operator<<(std::ostream& stream, const HVector3D& vec)
{
	stream << '(' << vec.x << ", " << vec.y << ", " << vec.z << ')' << std::endl;

	return stream;
}

std::ostream& operator<<(std::ostream& stream, const HVector2D& vec)
{
	stream << '(' << vec.x << ", " << vec.y  << ')' << std::endl;

	return stream;
}

int main()
{
	Transform2D transform(60.0f, 40.0f, 0.0f);

	HVector2D X(1.0f, 1.0f);
	HVector2D Y(1.0f, 2.0f);

	HVector2D v = transform.mat * X.mat;

	//std::cout << (X ^ Y) << std::endl;
	std::cout << v << std::endl;
	//std::cout << X.mat.Norm() << std::endl;

	HVector3D vec1(1.0f, 1.0f, 1.0f);
	HVector3D vec2(1.0f, 2.0f, 3.0f);

	std::cout << (vec1 | vec2) << std::endl;

	std::cin.get();
}

#endif // TEST04
