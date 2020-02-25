#include "Transform3D.h"

Transform3D::Transform3D() :
	mat(true)
{}

Transform3D::Transform3D(const Transform3D & _tr) :
	mat(_tr.mat)
{
}

Transform3D::Transform3D(const SMatrix44f & _mat) :
	mat(_mat)
{}

Transform3D::Transform3D(const SqrSMatrix44f & _mat) :
	mat(_mat)
{}
