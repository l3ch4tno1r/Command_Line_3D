#include "Transform3D.h"

Transform3D::Transform3D() :
	mat(true)
{}

Transform3D::Transform3D(const SqrSMatrix44f & _mat) :
	mat(_mat)
{}
