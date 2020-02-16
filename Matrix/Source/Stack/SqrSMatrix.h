#pragma once

#include "SMatrix.h"

namespace Matrix
{
	template<typename T, uint LC>
	class SqrSMatrix : public SMatrix<T, LC, LC>
	{
	public:
		/////////////////////////////////////
		//-- Constructos and destructors --//
		/////////////////////////////////////
		SqrSMatrix() : SMatrix()
		{}

		SqrSMatrix(bool) : SMatrix()
		{
			for (uint i = 0; i < LC; i++)
				for (uint j = 0; j < LC; j++)
					m_Matrix[i][j] = (i == j ? T(1) : T(0));
		}

		SqrSMatrix(T value) : SMatrix(value)
		{}

		SqrSMatrix(const T mat[LC][LC]) : SMatrix(mat)
		{}

		SqrSMatrix(const SMatrix& mat) : SMatrix(mat)
		{}

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
			static SqrSMatrix temp;
			temp = *this;

			return temp.GaussElimination();
		}

		////////////////////////
		//-- Static Methods --//
		////////////////////////
		static const SqrSMatrix& Identity()
		{
			static SqrSMatrix id(true);
			return id;
		}
	};
}