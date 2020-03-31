#include <iostream>

#include "Matrix\Stack\SqrSMatrix.h"

int main()
{
	using LCNMath::Matrix::StaticMatrix::Matrix;

	Matrix<float, 2, 3> mat1 = {
		1, 2, 3,
		4, 5, 6
	};

	std::cin.get();
}