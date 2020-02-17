#pragma once

#include "Stack\SqrSMatrix.h"

#define PI 3.14159265
#define TORAD(A) (PI * A) / 180

using SqrSMatrix33f = Matrix::SqrSMatrix<float, 3>;
using    SMatrix31f = Matrix::SMatrix<float, 3, 1>;

class Transform2D : public SqrSMatrix33f
{
public:
	Transform2D();

	Transform2D(float x, float y, float a);

	void SetRotationAngle(float a);

	void SetTranslation(float x, float y);
};

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