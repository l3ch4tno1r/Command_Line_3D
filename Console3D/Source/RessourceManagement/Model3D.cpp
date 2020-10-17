#include "Model3D.h"

Model3D::Model3D()
{}

Model3D::Model3D(Model3D&& _model) :
	m_Vertices(std::move(_model.m_Vertices)),
	m_Normals(std::move(_model.m_Normals)),
	m_Faces(std::move(_model.m_Faces)),
	m_Edges(std::move(_model.m_Edges))
{}

std::vector<Model3D::Vertex3D>& Model3D::Vertices()
{
	return m_Vertices;
}

std::vector<Model3D::Vertex3D>& Model3D::Normals()
{
	return m_Normals;
}

std::vector<Model3D::Face>& Model3D::Faces()
{
	return m_Faces;
}

std::unordered_set<Model3D::Edge, Model3D::HashEdge>& Model3D::Edges()
{
	return m_Edges;
}

const std::vector<Model3D::Vertex3D>& Model3D::Vertices() const
{
	return m_Vertices;
}

const std::vector<Model3D::Vertex3D>& Model3D::Normals() const
{
	return m_Normals;
}

const std::vector<Model3D::Face>& Model3D::Faces() const
{
	return m_Faces;
}

const std::unordered_set<Model3D::Edge, Model3D::HashEdge>& Model3D::Edges() const
{
	return m_Edges;
}
