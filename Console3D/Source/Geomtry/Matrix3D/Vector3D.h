#pragma once

#include "Matrix\Stack\SMatrix.h"

using SMatrix41f = Matrix::StaticMatrix::Matrix<float, 4, 1>;

union Vector3D
{
	struct
	{
		float x;
		float y;
		float z;
	};

	SMatrix41f mat;

	Vector3D();

	Vector3D(const Vector3D& vec);

	Vector3D(float _x, float _y, float _z);

	Vector3D(const SMatrix41f& _mat);
};

Vector3D operator^(const Vector3D& vec1, const Vector3D& vec2);