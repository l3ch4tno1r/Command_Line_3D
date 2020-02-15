#include "stdafx.h"
#include "CppUnitTest.h"

#include "Stack\SqrSMatrix.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(SqrSMatrixUT)
	{
		template<Matrix::uint LC >
		using SqrMatrixf = Matrix::SqrSMatrix<float, LC>;

	public:
		TEST_METHOD(UT01_Construction)
		{
			SqrMatrixf<3> mat1;

			Assert::AreEqual(3u, mat1.Lines());
			Assert::AreEqual(3u, mat1.Columns());
		}
	};
}