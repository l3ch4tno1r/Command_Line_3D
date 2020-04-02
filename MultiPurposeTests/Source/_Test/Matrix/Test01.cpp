#include <iostream>

#include "Matrix\Stack\SqrSMatrix.h"

using LCNMath::Matrix::StaticMatrix::Matrix;
using LCNMath::Matrix::StaticMatrix::SqrMatrix;
using LCNMath::Matrix::StaticMatrix::MatrixExpression;

using uint = unsigned int;

template<typename E, typename T, uint L, uint C>
void f(const MatrixExpression<E, T, L, C>& ME)
{
	std::cout << ME.Lines() << ", " << ME.Columns() << std::endl;
}

int main()
{
	SqrMatrix<float, 2> smat =
	{
		0, -1,
		1,  0
	};

	auto expr = 2.0f * smat;
	SqrMatrix<float, 2> result = 2.0f * smat;

	std::cin.get();
}