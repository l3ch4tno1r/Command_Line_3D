#include "Utils.h"

std::ostream& operator<<(std::ostream& stream, const HVector3D& vec)
{
	stream << '(' << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.s << ')';

	return stream;
}