#ifdef TEST12

#include <iostream>

#include "Geometry\HVectorND.h"

using LCNMath::Geometry::VectorND;
using LCNMath::Geometry::HVectorND;
using LCNMath::Matrix::StaticMatrix::Matrix;

int main()
{
	const VectorND<3>& UnitX = VectorND<3>::UnitVector<0>();
	const VectorND<3>& UnitY = VectorND<3>::UnitVector<1>();
	const VectorND<3>& UnitZ = VectorND<3>::UnitVector<2>();

	std::cin.get();
}

#endif // TEST12
