#pragma once

#include "Model3D.h"

#include <string>

class OBJReader
{
private:
	void ExtractData(const std::string& rawdata, uint& overtexidx, uint& onormalidx);
public:
	Model3D ReadFile(const std::string& filepath);
};