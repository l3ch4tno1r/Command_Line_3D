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