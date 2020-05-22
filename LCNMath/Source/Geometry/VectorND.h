#pragma once

#include <cmath>
#include <iostream>
#include <initializer_list>

#include "Matrix\Stack\SMatrix.h"

namespace LCNMath{
	namespace Geometry	{

		template<typename T, uint N>
		using MatrixN1 = LCNMath::Matrix::StaticMatrix::Matrix<T, N, 1>;

		template<typename T, uint N>
		class VectorND : public MatrixN1<T, N>
		{
		public:
			VectorND() :
				MatrixN1<T, N>(1.0f)
			{}
			
			VectorND(uint i) :
				MatrixN1<T, N>(0.0f)
			{
				(*this)[i] = 1.0f;
			}

			VectorND(std::initializer_list<float> _paramlist) :
				MatrixN1<T, N>(_paramlist)
			{}

			VectorND(const MatrixN1<T, N>& mat) :
				MatrixN1<T, N>(mat)
			{}

			T Norm() const
			{
				T norm = 0.0f;

				for (uint i = 0; i < N; i++)
					norm += (*this)(i, 0) * (*this)(i, 0);

				return std::sqrt(norm);
			}

			inline T& operator[](uint i)
			{
				return (*this)(i, 0);
			}

			inline T operator[](uint i) const
			{
				return (*this)(i, 0);
			}

			template<uint I>
			static const VectorND<T, N>& UnitVector()
			{
				static VectorND<T, N> result(I);
				return result;
			}
		};

		////////////////////////////
		//-- External Operators --//
		////////////////////////////
		template<typename T, uint N>
		T operator|(const VectorND<T, N>& vec1, const VectorND<T, N>& vec2)
		{
			float dotproduct = 0.0f;
		
			for (uint i = 0; i < N; i++)
				dotproduct += vec1(i, 0) * vec2(i, 0);
		
			return dotproduct;
		}

		template<typename T, uint N>
		std::ostream& operator<<(std::ostream& stream, const VectorND<T,N>& vec)
		{
			stream << '(';

			for (uint i = 0; i < N; i++)
				stream << vec[i] << (i < N - 1 ? ", " : "");

			stream << ')';

			return stream;
		}
	}
}