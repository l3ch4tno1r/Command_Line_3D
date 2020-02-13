#pragma once

namespace Matrix
{
	using uint = unsigned int;

	////////////////////////////////
	//-- Stack allocated Matrix --//
	////////////////////////////////
	template<typename T, uint L, uint C>
	class SMatrix
	{
	private:
		T m_Matrix[L][C];

	public:
		/////////////////////////////////////
		//-- Constructos and destructors --//
		/////////////////////////////////////
		SMatrix()
		{}

		SMatrix(T value)
		{
			for (uint i = 0; i < L; i++)
				for (uint j = 0; j < C; j++)
					m_Matrix[i][j] = value;
		}

		SMatrix(const T mat[L][C])
		{
			for (uint i = 0; i < L; i++)
				memcpy(m_Matrix[i], mat[i], C * sizeof(T));
		}

		SMatrix(const SMatrix& mat) :
			SMatrix(mat.m_Matrix)
		{}

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

		SMatrix& operator+=(const SMatrix &mat)
		{
			for (uint i = 0; i < L; i++)
				for (uint j = 0; j < C; j++)
					m_Matrix[i][j] += mat.m_Matrix[i][j];

			return *this;
		}

		SMatrix& operator-=(const SMatrix &mat)
		{
			for (uint i = 0; i < L; i++)
				for (uint j = 0; j < C; j++)
					m_Matrix[i][j] -= mat.m_Matrix[i][j];

			return *this;
		}
	};

	template<typename T, uint L, uint C>
	SMatrix<T, L, C> operator+(const SMatrix<T, L, C>& mat1, const SMatrix<T, L, C>& mat2)
	{
		SMatrix<T, L, C> result = mat1;

		result += mat2;

		return result;
	}

	template<typename T, uint L, uint C>
	SMatrix<T, L, C> operator-(const SMatrix<T, L, C>& mat1, const SMatrix<T, L, C>& mat2)
	{
		SMatrix<T, L, C> result = mat1;

		result -= mat2;

		return result;
	}

	template<typename T, uint L, uint LC, uint C>
	SMatrix<T, L, C> operator*(const SMatrix<T, L, LC>& mat1, const SMatrix<T, LC, C>& mat2)
	{
		SMatrix<T, L, C> result(T(0));

		for (uint i = 0; i < L; i++)
			for (uint j = 0; j < C; j++)
				for (uint k = 0; k < LC; k++)
					result(i, j) += mat1(i, k) * mat2(k, j);

		return result;
	}
}