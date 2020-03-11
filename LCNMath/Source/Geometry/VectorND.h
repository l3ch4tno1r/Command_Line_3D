#pragma once

#include <cmath>

#include "Matrix\Stack\SMatrix.h"

namespace LCNMath
{
	namespace Geometry
	{
		template<uint N>
		using MatrixN1f = Matrix::StaticMatrix::Matrix<float, N, 1>;

		template<uint N>
		class VectorND : public MatrixN1f<N>
		{
		public:
			VectorND() :
				MatrixN1f<N>(1.0f)
			{}

			VectorND(const MatrixN1f<N>& mat) :
				MatrixN1f<N>(mat)
			{}

			float Norm() const
			{
				float norm = 0.0f;

				for (uint i = 0; i < N; i++)
					norm += (*this)(i, 0) * (*this)(i, 0);

				return std::sqrt(norm);
			}
		};

		template<uint N>
		float operator|(const VectorND<N>& vec1, const VectorND<N>& vec2)
		{
			float dotproduct = 0.0f;
		
			for (uint i = 0; i < N; i++)
				dotproduct += vec1(i, 0) * vec2(i, 0);
		
			return dotproduct;
		}
	}
}