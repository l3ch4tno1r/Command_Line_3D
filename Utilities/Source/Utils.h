#pragma once

#include <iostream>

#include "Geometry\Geometry3D\HVector3D.h"
#include "Geometry\Geometry2D\HVector2D.h"

using namespace LCNMath::Geometry::Dim2;
using namespace LCNMath::Geometry::Dim3;

template<typename T>
short sign(T a)
{
	return (a > T(0)) - (a < T(0));
}

template<typename T>
std::ostream& operator<<(std::ostream& stream, const HVector2D<T>& vec);

template<typename T>
std::ostream& operator<<(std::ostream& stream, const HVector3D<T>& vec);