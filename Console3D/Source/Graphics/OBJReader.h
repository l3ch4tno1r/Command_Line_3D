#pragma once

#include "Model3D.h"

#include <string>
#include <fstream>
#include <exception>
#include <sstream>

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

		std::string line;

		while (std::getline(objfile, line))
		{
			std::stringstream sstr(line);

			std::string type;

			sstr >> type;

			if (type == "v")
			{
				HVector3D vec(true);

				sstr >> vec.x >> vec.y >> vec.z;

				result.Vertices().push_back(vec);
			}

			if(!generatenormals)
			{
				if (type == "vn")
				{
					HVector3D vec(false);

					sstr >> vec.x >> vec.y >> vec.z;

					result.Normals().push_back(vec);
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

					result.Faces().push_back(face);
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

					HVector3D v1 = result.Vertices()[face.v2] - result.Vertices()[face.v1];
					HVector3D v2 = result.Vertices()[face.v3] - result.Vertices()[face.v2];

					HVector3D n  = v1 ^ v2;

					float norm = n.x * n.x + n.y * n.y + n.z * n.z;

					n.x /= norm;
					n.y /= norm;
					n.z /= norm;

					result.Normals().push_back(n);

					face.vn1 = face.vn2 = face.vn3 = result.Normals().size() - 1;

					result.Faces().push_back(face);
				}
			}
		}

		return result;
	}
};