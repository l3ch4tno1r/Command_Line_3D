#pragma once

#include "VectorND.h"

namespace LCNMath
{
	namespace Geometry
	{
		template<uint N>
		struct HVectorND
		{
			union
			{
				struct
				{
					VectorND<N> vec;
					float s;
				};

				VectorND<N + 1> hvec;
			};

			HVectorND() :
				hvec()
			{}

			HVectorND(const HVectorND& _hvec) :
				hvec(_hvec.hvec)
			{}

			HVectorND(const VectorND<N>& _vec, float _s) :
				vec(_vec),
				s(_s)
			{}
		};

		template<uint N>
		inline HVectorND<N> operator+(const HVectorND<N>& a, const HVectorND<N>& b)
		{
			return HVectorND<N>(a.vec + b.vec, (a.s == 1.0f || b.s == 1.0f ? 1.0f : 0.0f));
		}

		template<uint N>
		inline std::ostream& operator<<(std::ostream& stream, const HVectorND<N>& vec)
		{
			stream << vec.vec << '(' << vec.s << ')';

			return stream;
		}
	}
}