#ifdef TEST09

#include <iostream>

#include "Utilities\Utils.h"

#include "Geometry\Geometry3D\HVector3D.h"

const float cm_ScreenDist = 0.5f;

void ScreenIntersect(HVector3D& a, HVector3D& b)
{
	if (sign(a.z - cm_ScreenDist) == sign(b.z - cm_ScreenDist))
		return;

	HVector3D& p1 = (a.z > 0.0f ? a : b);
	HVector3D& p2 = (a.z > 0.0f ? b : a);

	float dz = p2.z - p1.z;

	if (std::abs(dz) < 0.001f)
		return;

	float k = (cm_ScreenDist - p2.z) / dz;

	p2 = p2 + k * (p2 - p1);
}

int main()
{
	HVector3D a = { 2.0f, 0.0f, 0.0f };
	HVector3D b = { 0.0f, 0.0f, 1.0f };

	ScreenIntersect(a, b);

	std::cout << a << ", " << b << std::endl;

	std::cin.get();
}

#endif // TEST09
