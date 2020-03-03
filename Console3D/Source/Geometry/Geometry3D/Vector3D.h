#pragma once

#include "Geometry\VectorND.h"

using Vector4Df = VectorND<4>;

union HVector3D
{
	struct
	{
		float x;
		float y;
		float z;
		float s;
	};

	Vector4Df mat;

	HVector3D(bool ispoint);
		
	HVector3D(float _x, float _y, float _z, bool ispoint = true);

	HVector3D(const HVector3D& vec);

	HVector3D(const Vector4Df& _mat);

	HVector3D(const MatrixN1f<4>& _mat);

	static const HVector3D& X();

	static const HVector3D& Y();

	static const HVector3D& Z();
};

HVector3D operator^(const HVector3D& vec1, const HVector3D& vec2);

float operator|(const HVector3D& vec1, const HVector3D& vec2);