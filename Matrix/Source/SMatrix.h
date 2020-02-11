#pragma once

namespace Matrix
{
	////////////////////////////////
	//-- Stack allocated Matrix --//
	////////////////////////////////
	template<typename T, unsigned int L, unsigned int C>
	class SMatrix
	{
	private:
		unsigned int m_Lines;
		unsigned int m_Columns;

		T m_Matrix[L][C];

	public:
		SMatrix() :
			m_Lines(L),
			m_Columns(C)
		{}
	};
}