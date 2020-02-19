#pragma once

#include "Geometry\VectorND.h"

using Vector4Df = VectorND<4>;

union Vector3D
{
	struct
	{
		float x;
		float y;
		float z;
	};

	Vector4Df mat;

	Vector3D();
		
	Vector3D(float _x, float _y, float _z);

	Vector3D(const Vector3D& vec);

	Vector3D(const Vector4Df& _mat);
};

Vector3D operator^(const Vector3D& vec1, const Vector3D& vec2);

float operator*(const Vector3D& vec1, const Vector3D& vec2);