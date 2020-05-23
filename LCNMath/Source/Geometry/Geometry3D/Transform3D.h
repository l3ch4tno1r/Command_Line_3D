#pragma once

#include "Matrix\Stack\SqrSMatrix.h"

#include "HVector3D.h"

using namespace LCNMath::Geometry::Dim3;

template<typename T>
using SMatrix44 = LCNMath::Matrix::StaticMatrix::Matrix<T, 4, 4>;

template<typename T>
using SqrSMatrix44 = LCNMath::Matrix::StaticMatrix::SqrMatrix<T, 4>;

template<typename T>
struct Transform3D
{
	union
	{
		struct
		{
			T Rux, Rvx, Rwx, Tx,
			  Ruy, Rvy, Rwy, Ty,
			  Ruz, Rvz, Rwz, Tz;

			// Representation of :
			// [ Rux, Rvx, Rwx, Tx ]
			// [ Ruy, Rvy, Rwy, Ty ]
			// [ Ruz, Rvz, Rwz, Tz ]
			// [  0,   0,   0,   1 ]
		};

		SqrSMatrix44<T> mat;
	};

	Transform3D() :
		mat(true)
	{}

	Transform3D(const Transform3D& _tr) :
		mat(_tr.mat)
	{}

	Transform3D(const SMatrix44<T>& _mat) :
		mat(_mat)
	{}

	Transform3D(const SqrSMatrix44<T>& _mat) :
		mat(_mat)
	{}
};

template<typename T>
Transform3D<T> operator*(const Transform3D<T>& a, const Transform3D<T>& b)
{
	return a.mat * b.mat;
}

template<typename T>
HVector3D<T> operator*(const Transform3D<T>& t, const HVector3D<T>& v)
{
	return t.mat * v.mat;
}