#include "OBJReader.h"

void OBJReader::ExtractData(const std::string& rawdata, uint& overtexidx, uint& onormalidx)
{
	size_t separatoridx = rawdata.find('/');

	if (separatoridx == std::string::npos)
		throw std::exception("Bad format.");

	std::string str1 = rawdata.substr(0, separatoridx);
	std::string str2 = rawdata.substr(separatoridx + 1, rawdata.length() - 1);

	overtexidx = std::stoi(str1) - 1;
	onormalidx = std::stoi(str2) - 1;
}

/*
Model3D OBJReader::ReadFile(const std::string& filepath)
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
*/
