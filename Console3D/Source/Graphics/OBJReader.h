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
	C ReadFile(const std::string& filepath)
	{
		static_assert(false);
	}

	template <>
	Model3D ReadFile(const std::string& filepath)
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

		return result;
	}
};