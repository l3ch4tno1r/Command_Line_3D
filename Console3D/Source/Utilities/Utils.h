#pragma once

#include <iostream>

#include "Geometry\Geometry3D\HVector3D.h"

template<typename T>
short sign(T a)
{
	return (a > T(0)) - (a < T(0));
}

std::ostream& operator<<(std::ostream& stream, const HVector3D& vec);