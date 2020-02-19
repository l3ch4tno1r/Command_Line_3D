#pragma once

#include "Geometry\VectorND.h"

using Vector3Df = VectorND<3>;

union Vector2D
{
	struct
	{
		float x;
		float y;
	};

	Vector3Df mat;

	Vector2D();

	Vector2D(float _x, float _y);

	Vector2D(const Vector3Df& _mat);

	Vector2D(const MatrixN1f<3>& _mat);
};