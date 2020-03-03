#pragma once

#include <vector>

#include "Geometry\Geometry3D\Vector3D.h"

using uint = unsigned int;

class Model3D
{
public:
	using Vertex3D = HVector3D;

	struct Face
	{
		union
		{
			struct
			{
				uint  v1,  v2,  v3;
				uint vn1, vn2, vn3;
			};

			struct
			{
				uint Vertices[3];
				uint Normals[3];
			};
		};	
	};

private:
	std::vector<Vertex3D> m_Vertices;
	std::vector<Vertex3D> m_Normals;
	std::vector<Face>     m_Faces;

public:
	Model3D();

	Model3D(Model3D&& _model);

	std::vector<Vertex3D>& Vertices();
	std::vector<Vertex3D>& Normals();
	std::vector<Face>&     Faces();

	const std::vector<Vertex3D>& Vertices() const;
	const std::vector<Vertex3D>& Normals() const;
	const std::vector<Face>&     Faces() const;
};