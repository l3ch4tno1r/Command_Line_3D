#include "Vector3D.h"

Vector3D::Vector3D() :
	mat(1.0f)
{}

Vector3D::Vector3D(const Vector3D & vec) :
	mat(vec.mat)
{}

Vector3D::Vector3D(float _x, float _y, float _z, bool ispoint) :
	x(_x),
	y(_y),
	z(_z)
{
	mat(3, 0) = (ispoint ? 1.0f : 0.0f);
}

Vector3D::Vector3D(const Vector4Df& _mat) :
	mat(_mat)
{}

Vector3D::Vector3D(const MatrixN1f<4>& _mat) :
	mat(_mat)
{}

const Vector3D& Vector3D::X()
{
	static Vector3D x(1.0f, 0.0f, 0.0f);
	return x;
}

const Vector3D& Vector3D::Y()
{
	static Vector3D y(0.0f, 1.0f, 0.0f);
	return y;
}

const Vector3D& Vector3D::Z()
{
	static Vector3D z(0.0f, 0.0f, 1.0f);
	return z;
}

Vector3D operator^(const Vector3D & vec1, const Vector3D & vec2)
{
	static Vector3D result;

	result.x = vec1.y * vec2.z - vec1.z * vec2.y;
	result.y = vec1.z * vec2.x - vec1.x * vec2.z;
	result.z = vec1.x * vec2.y - vec1.y * vec2.x;

	return result;
}

float operator|(const Vector3D & vec1, const Vector3D & vec2)
{
	return vec1.mat | vec2.mat;
}
