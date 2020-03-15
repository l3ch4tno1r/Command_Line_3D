#include "Transform2D.h"

#include "Utilities\Angles.h"

#include <cmath>

Transform2D::Transform2D() :
	mat(true)
{}

Transform2D::Transform2D(const SMatrix33f & _mat) :
	mat(_mat)
{}

Transform2D::Transform2D(const SqrSMatrix33f& _mat) :
	mat(_mat)
{}

Transform2D::Transform2D(float x, float y, float a) :
	mat(true)
{
	SetTranslation(x, y);
	SetRotationAngle(a);
}

void Transform2D::SetRotationAngle(float a)
{
	Rux =  std::cos(TORAD(a));
	Ruy =  std::sin(TORAD(a));
	Rvx = -std::sin(TORAD(a));
	Rvy =  std::cos(TORAD(a));
}

void Transform2D::SetTranslation(float x, float y)
{
	Tx = x;
	Ty = y;
}