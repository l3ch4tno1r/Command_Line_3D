#pragma once

#include "SMatrix.h"

namespace LCNMath {
	namespace Matrix {
		namespace StaticMatrix {

			template<typename T, uint LC>
			class SqrMatrix : public Matrix<T, LC, LC>
			{
			public:
				using TMatrix = Matrix<T, LC, LC>;
				using TMatrix::m_Matrix;

#pragma region Constructors_Destructors
				//////////////////////////////////////
				//-- Constructors and destructors --//
				//////////////////////////////////////

				SqrMatrix() : TMatrix()
				{}

				SqrMatrix(bool)
				{
					for (uint i = 0; i < LC; i++)
						for (uint j = 0; j < LC; j++)
							m_Matrix[i][j] = (i == j ? T(1) : T(0));
				}

				SqrMatrix(T value) : TMatrix(value)
				{}

				SqrMatrix(const T mat[LC][LC]) : TMatrix(mat)
				{}

				SqrMatrix(const TMatrix& mat) : TMatrix(mat)
				{}

#pragma endregion

#pragma region Methods
				/////////////////
				//-- Methods --//
				/////////////////

				T Trace() const
				{
					T result(T(0));

					for (uint i = 0; i < LC; i++)
						result += m_Matrix[i][i];

					return result;
				}

				T Det() const
				{
					static SqrMatrix temp;
					temp = *this;

					return temp.GaussElimination();
				}

				SqrMatrix Invert() const
				{
					static Matrix<T, LC, 2 * LC> temp;

					temp.SubMatrix(*this, 0, 0);
					temp.SubMatrix(SqrMatrix::Identity(), 0, LC);

					T pseudodet = temp.GaussElimination();

					if (std::abs(pseudodet) < T(0.0001))
						throw std::exception("This matrix cannot be inverted.");

					return temp.SubMatrix<LC, LC>(0, LC);
				}

#pragma endregion

#pragma region Static_Methods
				////////////////////////
				//-- Static Methods --//
				////////////////////////

				static const SqrMatrix& Identity()
				{
					static SqrMatrix id(true);
					return id;
				}

#pragma endregion
			};
		}
	}
}