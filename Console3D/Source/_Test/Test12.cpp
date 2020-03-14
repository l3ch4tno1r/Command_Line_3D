#ifdef TEST12

#include <iostream>

#include "Geometry\HVectorND.h"

using LCNMath::Geometry::VectorND;
using LCNMath::Geometry::HVectorND;

int main()
{
	HVectorND<2> vec1 = { VectorND<2>(2.0f), 1.0f };
	HVectorND<2> vec2 = { VectorND<2>(3.0f), 1.0f };

	std::cin.get();
}

#endif // TEST12
