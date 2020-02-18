#include "Transform2D.h"

#include <cmath>

Transform2D::Transform2D() :
	mat(true)
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

Vector2D::Vector2D() :
	mat(1.0f)
{}

Vector2D::Vector2D(float _x, float _y) :
	x(_x),
	y(_y)
{
	mat(2, 0) = 1.0f;
}

Vector2D::Vector2D(const SMatrix31f & _mat) :
	mat(_mat)
{}