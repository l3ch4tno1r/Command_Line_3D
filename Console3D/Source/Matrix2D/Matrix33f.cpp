#include "Matrix33f.h"

#include <cmath>

Transform2D::Transform2D() :
	SqrSMatrix33f(true)
{}

Transform2D::Transform2D(float x, float y, float a) :
	SqrSMatrix33f(true)
{
	SetTranslation(x, y);
	SetRotationAngle(a);
}

void Transform2D::SetRotationAngle(float a)
{
	m_Matrix[0][0] =  std::cos(TORAD(a));
	m_Matrix[1][0] =  std::sin(TORAD(a));
	m_Matrix[0][1] = -std::sin(TORAD(a));
	m_Matrix[1][1] =  std::cos(TORAD(a));
}

void Transform2D::SetTranslation(float x, float y)
{
	m_Matrix[0][2] = x;
	m_Matrix[1][2] = y;
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