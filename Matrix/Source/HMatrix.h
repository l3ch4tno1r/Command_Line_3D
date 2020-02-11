#pragma once

namespace Matrix
{
	///////////////////////////////
	//-- Heap allocated Matrix --//
	///////////////////////////////
	template<typename T>
	class HMatrix
	{
	private:
		unsigned int m_Lines;
		unsigned int m_Columns;

		T** m_Matrix;

	public:
		HMatrix(unsigned int L, unsigned int C) :
			m_Lines(L),
			m_Columns(C),
			m_Matrix(nullptr)
		{
			m_Matrix = new T*[m_Lines];

			for (unsigned int i = 0; i < m_Lines; i++)
				m_Matrix[i] = new T[m_Columns];
		}

		virtual ~HMatrix()
		{
			for (unsigned int i = 0; i < m_Lines; i++)
				delete[] m_Matrix[i];

			delete[] m_Matrix;
		}
	};
}
