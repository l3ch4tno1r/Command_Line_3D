#pragma once

#include "Matrix\Stack\SqrSMatrix.h"
#include "Utilities\Angles.h"

namespace LCNMath {
	namespace Geometry {
		namespace Dim2 {

			template<typename T>
			using SMatrix33f    = LCNMath::Matrix::StaticMatrix::Matrix<T, 3, 3>;

			template<typename T>
			using SqrSMatrix33f = LCNMath::Matrix::StaticMatrix::SqrMatrix<T, 3>;

			template<typename T>
			union Transform2D
			{
				struct
				{
					float Rux, Rvx, Tx,
						  Ruy, Rvy, Ty;

					// Representation of :
					// [ Rux, Rvx, Tx ]
					// [ Ruy, Rvy, Ty ]
					// [  0,   0,   1 ]
			;	};

				SqrSMatrix33f<T> mat;

				Transform2D() :
					mat(true)
				{}

				Transform2D(const SMatrix33f<T>& _mat) :
					mat(_mat)
				{}

				Transform2D(const SqrSMatrix33f<T>& _mat) :
					mat(_mat)
				{}

				Transform2D(T x, T y, T a) :
					mat(true)
				{
					SetTranslation(x, y);
					SetRotationAngle(a);
				}

				void SetRotationAngle(float a)
				{
					Rux =  std::cos(TORAD(a));
					Ruy =  std::sin(TORAD(a));
					Rvx = -std::sin(TORAD(a));
					Rvy =  std::cos(TORAD(a));
				}

				void SetTranslation(T x, T y)
				{
					Tx = x;
					Ty = y;
				}
			};
		}
	}
}

