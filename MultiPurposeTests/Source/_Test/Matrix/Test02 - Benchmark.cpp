#include <iostream>

#include "Matrix\Stack\SqrSMatrix.h"
#include "Source\Utils\TimeMeasurement.h"
#include "Source\Utils\ErrorHandling.h"

using Vector3D = LCNMath::Matrix::StaticMatrix::Matrix<float, 3, 1>;

Vector3D NaiveAddition(const Vector3D& a, const Vector3D& b)
{
	return {
		a(0, 0) + b(0, 0),
		a(1, 0) + b(1, 0),
		a(2, 0) + b(2, 0),
	};
}

Vector3D a = {  1,  2,  3 };
Vector3D b = {  4,  5,  6 };
Vector3D c = {  7,  8,  9 };
Vector3D d = { 10, 11, 12 };

void TestLoopNaiveAddition()
{
	for (uint i = 0; i < 100000; ++i)
		float result = NaiveAddition(a, NaiveAddition(b, NaiveAddition(c, NaiveAddition(d, a))))(0, 0);
}

void TestLoopExprAddition()
{
	for (uint i = 0; i < 100000; ++i)
		float result = (a + b + c + d + a)(0, 0);
}

int main()
{
	Vector3D r1 = NaiveAddition(a, NaiveAddition(b, NaiveAddition(c, NaiveAddition(d, a))));
	Vector3D r2 = (a + b + c + d + a);

	ASSERT(r1 == r2);

	ASSERT(r1(0, 0) == r2(0, 0));
	ASSERT(r1(1, 0) == r2(1, 0));
	ASSERT(r1(2, 0) == r2(2, 0));

	MEASURETIME(TestLoopNaiveAddition());

	MEASURETIME(TestLoopExprAddition());

	std::cin.get();
}