#pragma once

#include <vector>
#include <unordered_set>

#include "Geometry\Geometry3D\HVector3D.h"

using namespace LCNMath::Geometry::Dim3;

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

	struct Edge
	{
		uint v1, v2;
		mutable uint n1, n2;

		Edge(uint v1, uint v2, uint n1 = 0, uint n2 = 0) :
			v1(v1 < v2 ? v1 : v2),
			v2(v1 < v2 ? v2 : v1),
			n1(n1),
			n2(n2)
		{}

		bool operator==(const Edge& other) const
		{
			return (v1 == other.v1) && (v2 == other.v2);
		}
	};

	struct HashEdge
	{
		size_t operator()(const Edge& edge) const
		{
			static std::hash<unsigned int> hashfct;

			return hashfct(edge.v1) + hashfct(edge.v2);
		}
	};

	struct EdgeEqual
	{
		bool operator()(const Edge& e1, const Edge& e2) const
		{
			return (e1.v1 == e2.v1) && (e1.v2 == e2.v2);
		}
	};

private:
	std::vector<Vertex3D>              m_Vertices;
	std::vector<Vertex3D>              m_Normals;
	std::vector<Face>                  m_Faces;
	std::unordered_set<Edge, HashEdge> m_Edges;

public:
	Model3D();

	Model3D(Model3D&& _model);

	std::vector<Vertex3D>&              Vertices();
	std::vector<Vertex3D>&              Normals();
	std::vector<Face>&                  Faces();
	std::unordered_set<Edge, HashEdge>& Edges();

	const std::vector<Vertex3D>&              Vertices() const;
	const std::vector<Vertex3D>&              Normals() const;
	const std::vector<Face>&                  Faces() const;
	const std::unordered_set<Edge, HashEdge>& Edges()const;
};