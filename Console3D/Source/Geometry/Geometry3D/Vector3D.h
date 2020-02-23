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
		float s;
	};

	Vector4Df mat;

	Vector3D();
		
	Vector3D(float _x, float _y, float _z, bool ispoint = true);

	Vector3D(const Vector3D& vec);

	Vector3D(const Vector4Df& _mat);

	Vector3D(const MatrixN1f<4>& _mat);

	static const Vector3D& X();

	static const Vector3D& Y();

	static const Vector3D& Z();
};

Vector3D operator^(const Vector3D& vec1, const Vector3D& vec2);

float operator|(const Vector3D& vec1, const Vector3D& vec2);