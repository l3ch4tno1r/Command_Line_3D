#include "Vector2D.h"

HVector2D::HVector2D() :
	mat(1.0f)
{}

HVector2D::HVector2D(float _x, float _y) :
	x(_x),
	y(_y)
{
	mat(2, 0) = 1.0f;
}

HVector2D::HVector2D(const Vector3Df & _mat) :
	mat(_mat)
{}

HVector2D::HVector2D(const MatrixN1f<3>& _mat) :
	mat(_mat)
{}
