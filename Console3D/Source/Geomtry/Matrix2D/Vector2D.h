#pragma once

#include "Matrix\Stack\SMatrix.h"

using SMatrix31f = Matrix::StaticMatrix::Matrix<float, 3, 1>;

union Vector2D
{
	struct
	{
		float x;
		float y;
	};

	SMatrix31f mat;

	Vector2D();

	Vector2D(float _x, float _y);

	Vector2D(const SMatrix31f& _mat);
};