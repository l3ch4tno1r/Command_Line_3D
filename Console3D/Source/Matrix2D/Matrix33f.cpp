#include "Matrix33f.h"

#include <cmath>

Matrix33f::Matrix33f() :
	SqrSMatrix33f(true)
{}

Matrix33f::Matrix33f(float x, float y, float a) :
	SqrSMatrix33f(true)
{
	SetTranslation(x, y);
	SetRotationAngle(a);
}

void Matrix33f::SetRotationAngle(float a)
{
	m_Matrix[0][0] =  std::cos(TORAD(a));
	m_Matrix[1][0] =  std::sin(TORAD(a));
	m_Matrix[0][1] = -std::sin(TORAD(a));
	m_Matrix[1][1] =  std::cos(TORAD(a));
}

void Matrix33f::SetTranslation(float x, float y)
{
	m_Matrix[0][2] = x;
	m_Matrix[1][2] = y;
}

Matrix31f::Matrix31f() :
	SMatrix31f(1.0f)
{}

Matrix31f::Matrix31f(float x, float y) :
	SMatrix31f(1.0f)
{
	m_Matrix[0][0] = x;
	m_Matrix[1][0] = y;
}

Matrix31f::Matrix31f(const SMatrix31f & mat) :
	SMatrix31f(mat)
{}

float& Matrix31f::X()
{
	return m_Matrix[0][0];
}

const float & Matrix31f::X() const
{
	return m_Matrix[0][0];
}

float& Matrix31f::Y()
{
	return m_Matrix[1][0];
}

const float & Matrix31f::Y() const
{
	return m_Matrix[1][0];
}
