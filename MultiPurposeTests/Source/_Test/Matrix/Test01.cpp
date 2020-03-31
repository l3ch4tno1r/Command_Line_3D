#include <iostream>

#include "Matrix\Stack\SqrSMatrix.h"

using LCNMath::Matrix::StaticMatrix::Matrix;
using LCNMath::Matrix::StaticMatrix::MatrixExpression;

template<typename E, typename T>
void f(const MatrixExpression<E, T>& ME)
{
	std::cout << ME.Lines() << ", " << ME.Columns() << std::endl;
}

int main()
{
	Matrix<float, 2, 3> mat1 = {
		1, 2, 3,
		4, 5, 6
	};

	Matrix<float, 2, 3> mat2 = {
		6, 5, 4,
		3, 2, 1
	};

	f(mat1);

	std::cout << (mat1 + mat2)(0, 0) << std::endl;

	std::cin.get();
}