#include "stdafx.h"
#include "CppUnitTest.h"

#include "Stack\SqrSMatrix.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MatrixUT
{
	TEST_CLASS(SqrSMatrixUT)
	{
		template<Matrix::uint L, Matrix::uint C >
		using Matrixf = Matrix::SMatrix<float, L, C>;

		template<Matrix::uint LC >
		using SqrMatrixf = Matrix::SqrSMatrix<float, LC>;

	public:
		TEST_METHOD(UT01_Construction)
		{
			const float tab[3][3] = {
				{ 1.0f, 2.0f, 3.0f },
				{ 4.0f, 5.0f, 6.0f },
				{ 7.0f, 8.0f, 9.0f }
			};

			SqrMatrixf<3> mat1(tab);

			Assert::AreEqual(3u, mat1.Lines());
			Assert::AreEqual(3u, mat1.Columns());

			for (unsigned int i = 0; i < 6; i++)
			{
				unsigned int I = i / 3;
				unsigned int J = i % 3;

				Assert::AreEqual((float)(i + 1), mat1(I, J));
			}

			SqrMatrixf<3> mat2(mat1);

			Assert::IsTrue(mat1 == mat2);

			auto& id = SqrMatrixf<3>::Identity();

			for (unsigned int i = 0; i < 6; i++)
			{
				unsigned int I = i / 3;
				unsigned int J = i % 3;

				Assert::AreEqual((I == J ? 1.0f : 0.0f), id(I, J));
			}
		}

		TEST_METHOD(UT02_Trace)
		{
			const float tab[3][3] = {
				{ 1.0f, 2.0f, 3.0f },
				{ 4.0f, 5.0f, 6.0f },
				{ 7.0f, 8.0f, 9.0f }
			};

			SqrMatrixf<3> mat = tab;

			Assert::AreEqual(15.0f, mat.Trace());

			mat(0, 0) = 4.0f;

			Assert::AreEqual(18.0f, mat.Trace());
		}

		TEST_METHOD(UT03_GaussElimination)
		{
			const float tab[3][3] = {
				{ 1.0f, 0.0f,  0.0f },
				{ 0.0f, 0.0f, -1.0f },
				{ 0.0f, 1.0f,  0.0f }
			};

			SqrMatrixf<3> mat = tab;

			Matrixf<3, 6> temp;

			temp.SubMatrix(mat, 0, 0);
			temp.SubMatrix(SqrMatrixf<3>::Identity(), 0, 3);

			temp.GaussElimination();

			SqrMatrixf<3> inv = temp.SubMatrix<3, 3>(0, 3);
			SqrMatrixf<3> res = mat * inv;

			Assert::IsTrue(res == SqrMatrixf<3>::Identity());
		}
	};
}