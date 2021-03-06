#pragma once

#include "Model3D.h"
#include "Utilities/Source/ErrorHandling.h"
#include "LCN_Math/Source/Matrix/MatrixBlock.h"

#include <string>
#include <fstream>
#include <exception>
#include <sstream>
#include <cmath>

namespace LCN::Ressource
{
	class OBJReader
	{
	private:
		void ExtractData(const std::string& rawdata, uint& overtexidx, uint& onormalidx);

	public:
		template <class C>
		C ReadFile(const std::string& filepath, bool generatenormals)
		{
			static_assert(false);
		}

		template <>
		Model3D ReadFile(const std::string& filepath, bool generatenormals)
		{
			std::ifstream objfile(filepath, std::ios::in);

			if (!objfile)
				throw std::exception("Impossible to open the OBJ file.");

			Model3D result;

			auto& Vertices = result.Vertices();
			auto& Faces    = result.Faces();
			auto& Normals  = result.Normals();
			auto& Edges    = result.Edges();

			std::string line;

			while (std::getline(objfile, line))
			{
				std::stringstream sstr(line);

				std::string type;

				sstr >> type;

				// Fill vertices array
				if (type == "v")
				{
					LCN::HVector3Df hvec;

					sstr >> hvec.x() >> hvec.y() >> hvec.z();

					hvec.w() = 1.0f;

					Vertices.push_back(hvec);
				}

				// Fill faces
				if(!generatenormals)
				{
					if (type == "vn")
					{
						LCN::HVector3Df hvec;

						sstr >> hvec.x() >> hvec.y() >> hvec.z();

						hvec.w() = 1.0f;

						Normals.push_back(hvec);
					}

					if (type == "f")
					{
						Model3D::Face face;
						std::string temp;

						sstr >> temp;
						ExtractData(temp, face.v1, face.vn1);

						sstr >> temp;
						ExtractData(temp, face.v2, face.vn2);

						sstr >> temp;
						ExtractData(temp, face.v3, face.vn3);

						Faces.push_back(face);

						/*
						Edges.insert({ face.v1, face.v2 });
						Edges.insert({ face.v2, face.v3 });
						Edges.insert({ face.v3, face.v1 });
						*/

						for (uint i = 0; i < 3; ++i)
						{
							auto edge = Edges.find({ face.Vertices[i], face.Vertices[(i + 1) % 3] });

							if (edge == Edges.end())
								Edges.insert({ face.Vertices[i], face.Vertices[(i + 1) % 3], face.vn1 });
							else
								edge->n2 = face.vn1;
						}
					}
				}
				else
				{
					if (type == "f")
					{
						Model3D::Face face;

						sstr >> face.v1 >> face.v2 >> face.v3;

						face.v1--;
						face.v2--;
						face.v3--;

						// /!\ Assumes that the vertices vector has been filled /!\

						LCN::HVector3Df v1 = Vertices[face.v2] - Vertices[face.v1];
						LCN::HVector3Df v2 = Vertices[face.v3] - Vertices[face.v2];

						LCN::HVector3Df n;
					
						n.Vector() = v1.Vector() ^ v2.Vector();
						n.w() = 0.0f;

						float norm = std::sqrt(n.x() * n.x() + n.y() * n.y() + n.z() * n.z());

						n.x() /= norm;
						n.y() /= norm;
						n.z() /= norm;

						Normals.push_back(n);

						face.vn1 = face.vn2 = face.vn3 = Normals.size() - 1;

						Faces.push_back(face);

						/*
						Edges.insert({ face.v1, face.v2 });
						Edges.insert({ face.v2, face.v3 });
						Edges.insert({ face.v3, face.v1 });
						*/

						for (uint i = 0; i < 3; ++i)
						{
							auto edge = Edges.find({ face.Vertices[i], face.Vertices[(i + 1) % 3] });

							if (edge == Edges.end())
								Edges.insert({ face.Vertices[i], face.Vertices[(i + 1) % 3], face.vn1 });
							else
								edge->n2 = face.vn1;
						}
					}
				}
			}

			return result;
		}
	};
}