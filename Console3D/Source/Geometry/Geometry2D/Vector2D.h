#pragma once

#include "Geometry\VectorND.h"

using Vector3Df = VectorND<3>;

union HVector2D
{
	struct
	{
		float x;
		float y;
		float s;
	};

	Vector3Df mat;

	HVector2D();

	HVector2D(float _x, float _y);

	HVector2D(const Vector3Df& _mat);

	HVector2D(const MatrixN1f<3>& _mat);

	inline float PX() const
	{
		return x / s;
	}

	inline float PY() const
	{
		return y / s;
	}
};