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

		TEST_METHOD(UT03_Determinant)
		{
			const float tab[3][3] = {
				{  2.0f, -1.0f,  0.0f },
				{  0.0f, -1.0f,  2.0f },
				{ -1.0f,  2.0f, -1.0f }
			};

			SqrMatrixf<3> mat = tab;

			float pseudodet = mat.Det();

			Assert::IsTrue(std::abs(-4.0f - pseudodet) < 0.001);
			Assert::IsTrue(std::abs(-4.0f - pseudodet) < 0.0001);
			Assert::IsTrue(std::abs(-4.0f - pseudodet) < 0.00001);
			Assert::IsTrue(std::abs(-4.0f - pseudodet) < 0.000001);
		}

		TEST_METHOD(UT04_InvertMatrix)
		{
			const float tab1[3][3] = {
				{ 1.0f, 0.0f,  0.0f },
				{ 0.0f, 0.0f, -1.0f },
				{ 0.0f, 1.0f,  0.0f }
			};

			SqrMatrixf<3> mat = tab1;

			SqrMatrixf<3> inv = mat.Invert();
			SqrMatrixf<3> res = mat * inv;

			Assert::IsTrue(res == SqrMatrixf<3>::Identity());
		}

		TEST_METHOD(UT05_2DTransform)
		{
			const float tab1[3][3] = {
				{ 0.0f, -1.0f, 7.0f },
				{ 1.0f,  0.0f, 7.0f },
				{ 0.0f,  0.0f, 1.0f }
			};

			const float tab2[3][1] = {
				{ 5.0f },
				{ 4.0f },
				{ 1.0f }
			};

			SqrMatrixf<3> mat = tab1;
			Matrixf<3, 1> vec = tab2;

			Matrixf<3, 1> res = mat * vec;

			Assert::AreEqual( 3.0f, res(0, 0));
			Assert::AreEqual(12.0f, res(1, 0));
		}
	};
}