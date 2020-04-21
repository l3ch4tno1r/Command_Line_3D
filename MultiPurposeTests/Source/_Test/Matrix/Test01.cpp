#include <iostream>

#include "Matrix\Stack\SqrSMatrix.h"

using LCNMath::Matrix::StaticMatrix::Matrix;
using LCNMath::Matrix::StaticMatrix::SqrMatrix;
using LCNMath::Matrix::StaticMatrix::MatrixExpression;

using uint = unsigned int;

template<typename E, typename T>
class Vector3DExpression : public MatrixExpression<Vector3DExpression<E, T>, T, 3, 1>
{
public:
	T operator[](uint i) const
	{
		return static_cast<const E&>(*this)[i];
	}

	T operator()(uint i, uint j) const
	{
		return (*this)[i];
	}
};

struct Vector3D : public Vector3DExpression<Vector3D, float>
{
	union
	{
		struct
		{
			float x, y, z;
		};
		float m_Matrix[3];
	};

	Vector3D(float x, float y, float z) :
		x(x),
		y(y),
		z(z)
	{}

	template<typename E>
	Vector3D(const MatrixExpression<E, float, 3, 1>& e)
	{
		for (uint i = 0; i < 3; ++i)
			m_Matrix[i] = e(i, 0);
	}

	float operator[](uint i) const
	{
		return m_Matrix[i];
	}

	float operator()(uint i, uint j) const
	{
		return m_Matrix[i];
	}
};

template<typename EL, typename ER>
class Vector3DCrossProduct : public Vector3DExpression<Vector3DCrossProduct<EL, ER>, float>
{
private:
	const EL& el;
	const ER& er;

public:
	Vector3DCrossProduct(const EL& el, const ER& er) :
		el(el),
		er(er)
	{}

	float operator[](uint i) const
	{
		unsigned int ip1 = (i + 1) % 3;
		unsigned int ip2 = (i + 2) % 3;

		return el[ip1] * er[ip2] - el[ip2] * er[ip1];
	}
};

template<typename EL, typename ER>
Vector3DCrossProduct<EL, ER> operator^(const MatrixExpression<EL, float, 3, 1>& el, const MatrixExpression<ER, float, 3, 1>& er)
{
	return Vector3DCrossProduct<EL, ER>(static_cast<const EL&>(el), static_cast<const ER&>(er));
}

template<typename EL, typename ER>
float operator|(const Vector3DExpression<EL, float>& el, const Vector3DExpression<ER, float>& er)
{
	float dotprod = 0;

	for (uint i = 0; i < 3; i++)
		dotprod += el[i] * er[i];

	return dotprod;
}


int main()
{
	SqrMatrix<float, 3> smat =
	{
		0, -1, 0,
		1,  0, 0,
		0,  0, 1
	};

	Vector3D UX = { -2, -3, -1.5 };
	Vector3D UY = {  0,  0,  1   };

	Vector3D result = UX ^ UY;

	std::cout << result[0] << std::endl;
	std::cout << result[1] << std::endl;
	std::cout << result[2] << std::endl;

	float d = (UX ^ UY) | UX;

	Vector3D v = (smat * UX) ^ UY;

	std::cin.get();
}