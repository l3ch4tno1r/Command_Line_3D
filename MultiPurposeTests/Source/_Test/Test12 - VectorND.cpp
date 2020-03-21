#ifdef TEST12

#include <iostream>

#include "Geometry\HVectorND.h"

using LCNMath::Geometry::VectorND;
using LCNMath::Geometry::HVectorND;
using LCNMath::Matrix::StaticMatrix::Matrix;

int main()
{
	/*
	VectorND<3> vec = { 1, 2, 3 };
	*/
	Matrix<float, 2, 3> mat = {
		1, 2, 3,
		4, 5, 6
	};

	std::cin.get();
}

#endif // TEST12
