#pragma once

#include <vector>

#include "Geometry\Geometry3D\Vector3D.h"

using uint = unsigned int;

class Model3D
{
public:
	struct Face
	{
		uint  v1,  v2,  v3;
		uint vn1, vn2, vn3;
	};

private:
	std::vector<Vector3D> m_Vertices;
	std::vector<Vector3D> m_Normals;
	std::vector<Face>     m_Faces;

public:
	Model3D();

	Model3D(Model3D&& _model);

	std::vector<Vector3D>& Vertices();
	std::vector<Vector3D>& Normals();
	std::vector<Face>&     Faces();
};