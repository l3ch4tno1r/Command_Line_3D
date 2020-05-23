#include "Utils.h"

template<typename T>
std::ostream& operator<<(std::ostream& stream, const HVector2D<T>& vec)
{
	stream << '(' << vec.x << ", " << vec.y << ", (" << vec.s << "))";

	return stream;
}

template<typename T>
std::ostream& operator<<(std::ostream& stream, const HVector3D<T>& vec)
{
	stream << '(' << vec.x << ", " << vec.y << ", " << vec.z << ", (" << vec.s << "))";

	return stream;
}