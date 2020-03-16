#pragma once

#include <algorithm>
#include <stdexcept>

using uint = unsigned int;

namespace LCNMath {
	namespace StaticMatrix{

		////////////////////////////////
		//-- Stack allocated Matrix --//
		////////////////////////////////
		template<typename T, uint L, uint C>
		class Matrix
		{
		protected:
			T m_Matrix[L][C];

		public:
#pragma region Constructors_Destructors
		//////////////////////////////////////
		//-- Constructors and destructors --//
		//////////////////////////////////////

		Matrix()
		{}

		Matrix(T value)
		{
			for (uint i = 0; i < L; i++)
				for (uint j = 0; j < C; j++)
					m_Matrix[i][j] = value;
		}

		Matrix(const T mat[L][C])
		{
			for (uint i = 0; i < L; i++)
				memcpy(m_Matrix[i], mat[i], C * sizeof(T));
		}

		Matrix(const Matrix& mat) :
			Matrix(mat.m_Matrix)
		{}

#pragma endregion

#pragma region Accessors
		///////////////////
		//-- Accessors --//
		///////////////////

		uint Lines() const
		{
			return L;
		}

		uint Columns() const
		{
			return C;
		}

		T& operator()(uint i, uint j)
		{
			if (i >= L || j >= C)
				throw std::out_of_range("Index out of range.");

			return m_Matrix[i][j];
		}

		const T& operator()(uint i, uint j) const
		{
			if (i >= L || j >= C)
				throw std::out_of_range("Index out of range.");

			return m_Matrix[i][j];
		}

		template<uint L2, uint C2>
		Matrix<T, L2, C2> SubMatrix(uint posi, uint posj) const
		{
			if(posi + L2 > L || posj + C2 > C)
				throw std::out_of_range("Index out of range.");

			static Matrix<T, L2, C2> result;

			for (uint i = 0; i < L2; i++)
				for (uint j = 0; j < C2; j++)
					result(i, j) = m_Matrix[i + posi][j + posj];

			return result;
		}

		template<uint L2, uint C2>
		void SubMatrix(const Matrix<T, L2, C2>& mat, uint posi, uint posj)
		{
			if (posi + L2 > L || posj + C2 > C)
				throw std::out_of_range("Index out of range.");

			for (uint i = 0; i < L2; i++)
				for (uint j = 0; j < C2; j++)
					m_Matrix[i + posi][j + posj] = mat(i, j);
		}

#pragma endregion

#pragma region Methods
		/////////////////
		//-- Methods --//
		/////////////////

		void SwapLines(uint i, uint j)
		{
			if (i >= L || j >= L)
				throw std::out_of_range("Index out of range.");

			T temp;

			for (int k = 0; k < C; k++)
			{
				temp = m_Matrix[i][k];

				m_Matrix[i][k] = m_Matrix[j][k];
				m_Matrix[j][k] = temp;
			}
		}

		void ScaleLine(uint idx, T scalefactor)
		{
			if (idx >= L)
				throw std::out_of_range("Index out of range.");

			for (uint j = 0; j < C; j++)
				m_Matrix[idx][j] *= scalefactor;
		}

		void CombineLines(uint idx1, T factor1, uint idx2, T factor2)
		{
			if(idx1 >= L || idx2 >= L)
				throw std::out_of_range("Index out of range.");

			for (uint j = 0; j < C; j++)
				m_Matrix[idx1][j] = factor1 * m_Matrix[idx1][j] + factor2 * m_Matrix[idx2][j];
		}

		T GaussElimination()
		{
			uint linepivot    = 0;
			uint permutations = 0;
			T    pseudodet(1);

			for (uint j = 0; j < std::min(L, C); j++)
			{
				// Recherche du pivot
				T   max    = 0;
				int maxpos = 0;

				for (uint i = linepivot; i < L; i++)
				{
					if (std::abs(m_Matrix[i][j]) > max)
					{
						max    = abs(m_Matrix[i][j]);
						maxpos = i;
					}
				}

				// maxpos est le pivot
				if (m_Matrix[maxpos][j] == 0)
					return T(0);

				pseudodet *= m_Matrix[maxpos][j];

				ScaleLine(maxpos, T(1) / (m_Matrix[maxpos][j]));

				if (maxpos != j)
				{
					SwapLines(maxpos, linepivot);
					permutations++;
				}

				for (uint i = 0; i < L; i++)
					if (i != linepivot)
						CombineLines(i, T(1), linepivot, -m_Matrix[i][j]);

				linepivot++;
			}

			return (permutations % 2 == 0 ? T(1): T(-1)) * pseudodet;
		}

#pragma endregion

#pragma region Operators_Overload
		////////////////////////////
		//-- Operators overload --//
		////////////////////////////

		Matrix& operator=(const Matrix& mat)
		{
			for (uint i = 0; i < L; i++)
				memcpy(m_Matrix[i], mat.m_Matrix[i], C * sizeof(T));

			return *this;
		}

		Matrix& operator+=(const Matrix& mat)
		{
			for (uint i = 0; i < L; i++)
				for (uint j = 0; j < C; j++)
					m_Matrix[i][j] += mat.m_Matrix[i][j];

			return *this;
		}

		Matrix& operator-=(const Matrix& mat)
		{
			for (uint i = 0; i < L; i++)
				for (uint j = 0; j < C; j++)
					m_Matrix[i][j] -= mat.m_Matrix[i][j];

			return *this;
		}

		bool operator==(const Matrix& mat) const
		{
			for (uint i = 0; i < L; i++)
				for (uint j = 0; j < C; j++)
					if (m_Matrix[i][j] != mat.m_Matrix[i][j])
						return false;

			return true;
		}

		bool operator!=(const Matrix& mat) const
		{
			return !(*this == mat);
		}

#pragma endregion

#pragma region Static_Methods
		////////////////////////
		//-- Static Methods --//
		////////////////////////

		static const Matrix& Zero()
		{
			static Matrix zero(T(0));
			return zero;
		}

#pragma endregion
	};

#pragma region External_Functions
		////////////////////////////
		//-- External functions --//
		////////////////////////////

		template<typename T, uint L, uint C>
		Matrix<T, L, C> operator+(const Matrix<T, L, C>& mat1, const Matrix<T, L, C>& mat2)
		{
			Matrix<T, L, C> result = mat1;

			result += mat2;

			return result;
		}

		template<typename T, uint L, uint C>
		Matrix<T, L, C> operator-(const Matrix<T, L, C>& mat1, const Matrix<T, L, C>& mat2)
		{
			Matrix<T, L, C> result = mat1;

			result -= mat2;

			return result;
		}

		template<typename T, uint L, uint LC, uint C>
		Matrix<T, L, C> operator*(const Matrix<T, L, LC>& mat1, const Matrix<T, LC, C>& mat2)
		{
			Matrix<T, L, C> result(T(0));

			for (uint i = 0; i < L; i++)
				for (uint j = 0; j < C; j++)
					for (uint k = 0; k < LC; k++)
						result(i, j) += mat1(i, k) * mat2(k, j);

			return result;
		}
#pragma endregion
	}
}