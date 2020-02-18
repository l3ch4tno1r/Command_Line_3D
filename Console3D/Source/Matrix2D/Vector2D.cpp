#include "Vector2D.h"

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