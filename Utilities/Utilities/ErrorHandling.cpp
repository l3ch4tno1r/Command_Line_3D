#include "ErrorHandling.h"

#include <sstream>

std::exception buildexception(const char* expr, const char* file, int line)
{
	std::stringstream msg;

	msg << expr << " | " << file << " | " << line;

	return std::exception(msg.str().c_str());
}
