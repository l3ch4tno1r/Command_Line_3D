#ifdef TEST13

#include <iostream>

#include "Geometry\Geometry2D\Transform2D.h"
#include "Geometry\Geometry2D\HVector2D.h"

int main()
{
	Transform2D R0R1;
	Transform2D Rot;

	R0R1.SetTranslation(1.0f, 1.0f);

	Rot.SetRotationAngle(90.0);

	Transform2D R1R0 = R0R1.mat.Invert();

	Transform2D TR = R0R1.mat * Rot.mat * R1R0.mat;

	const float tab[3][3] = {
		1.0f, 2.0f, 3.0f,
		4.0f, 5.0f, 6.0f,
		7.0f, 8.0f, 9.0f
	};

	LCNMath::StaticMatrix::SqrMatrix<float, 3> mat = tab;
	LCNMath::StaticMatrix::SqrMatrix<float, 3> res;

	for (uint i = 0; i < 3; i++)
	{
		for (uint j = 0; j < 3; j++)
		{
			HVector2D vec = TR.mat * HVector2D(i, j).mat;

			res(i, j) = mat((uint)vec.x, (uint)vec.y);
		}
	}

	std::cin.get();
}

#endif // TEST13
