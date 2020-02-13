#include "stdafx.h"
#include "CppUnitTest.h"
#include "SMatrix.h"
#include "HMatrix.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{		
	TEST_CLASS(SMatrixUT)
	{
		template<unsigned int L, unsigned int C>
		using Matrixf = Matrix::SMatrix<float, L, C>;

	public:		
		TEST_METHOD(UT01_DefaultConstructor)
		{
			Matrix::SMatrix<float, 3, 4> mat1;
			Matrix::SMatrix<float, 8, 6> mat2;

			Assert::AreEqual(3u, mat1.Lines());
			Assert::AreEqual(4u, mat1.Columns());

			Assert::AreEqual(8u, mat2.Lines());
			Assert::AreEqual(6u, mat2.Columns());

			Matrix::SMatrix<float, 2, 3> mat3(0.0f);

			for (unsigned int i = 0; i < 6; i++)
			{
				unsigned int I = i / 3;
				unsigned int J = i % 3;

				Assert::AreEqual(0.0f, mat3(I, J));
			}
		}

		TEST_METHOD(UT02_CopyConstructor)
		{
			const float tab[2][3] = {
				{ 1.0f, 2.0f, 3.0f },
				{ 4.0f, 5.0f, 6.0f }
			};

			Matrix::SMatrix<float, 2, 3> mat1 = tab;

			Assert::AreEqual(2u, mat1.Lines());
			Assert::AreEqual(3u, mat1.Columns());

			for (unsigned int i = 0; i < 6; i++)
			{
				unsigned int I = i / 3;
				unsigned int J = i % 3;

				Assert::AreEqual((float)(i + 1), mat1(I, J));
			}

			Matrix::SMatrix<float, 2, 3> mat2 = mat1;

			Assert::AreEqual(2u, mat2.Lines());
			Assert::AreEqual(3u, mat2.Columns());

			for (unsigned int i = 0; i < 6; i++)
			{
				unsigned int I = i / 3;
				unsigned int J = i % 3;

				Assert::AreEqual((float)(i + 1), mat2(I, J));
			}
		}

		TEST_METHOD(UT03_DataAccessOutOfRange)
		{
			const float tab[2][3] = {
				{ 1.0f, 2.0f, 3.0f },
				{ 4.0f, 5.0f, 6.0f }
			};

			Matrix::SMatrix<float, 2, 3> mat = tab;
			
			Assert::ExpectException<std::out_of_range>([&mat]() { return mat( 3,  0); });
			Assert::ExpectException<std::out_of_range>([&mat]() { return mat(-1,  0); });
			Assert::ExpectException<std::out_of_range>([&mat]() { return mat( 0,  3); });
			Assert::ExpectException<std::out_of_range>([&mat]() { return mat( 0, -1); });
		}

		TEST_METHOD(UT04_DataAccessReadWrite)
		{
			const float tab[2][3] = {
				{ 1.0f, 2.0f, 3.0f },
				{ 4.0f, 5.0f, 6.0f }
			};

			Matrix::SMatrix<float, 2, 3> mat = tab;

			mat(0, 0) = 4;

			Assert::AreEqual(4.0f, mat(0, 0));

			const Matrix::SMatrix<float, 2, 3>& ref = mat;

			mat(0, 1) = -2;

			Assert::AreEqual(-2.0f, mat(0, 1));
			Assert::AreEqual(-2.0f, ref(0, 1));
		}

		TEST_METHOD(UT05_SwapLines)
		{
			const float tab[2][3] = {
				{ 1.0f, 2.0f, 3.0f },
				{ 4.0f, 5.0f, 6.0f }
			};

			Matrix::SMatrix<float, 2, 3> mat = tab;

			mat.SwapLines(0, 1);

			for (unsigned int i = 0; i < 3; i++)
			{
				Assert::AreEqual(tab[0][i], mat(1, i));
				Assert::AreEqual(tab[1][i], mat(0, i));
			}

			Assert::ExpectException<std::out_of_range>([&mat]() { mat.SwapLines(0, 2); });
			Assert::ExpectException<std::out_of_range>([&mat]() { mat.SwapLines(2, 0); });
		}

		TEST_METHOD(UT06_AddMatrix)
		{
			const float tab1[2][3] = {
				{ 1.0f, 2.0f, 3.0f },
				{ 4.0f, 5.0f, 6.0f }
			};

			const float tab2[2][3] = {
				{ 6.0f, 5.0f, 4.0f },
				{ 3.0f, 2.0f, 1.0f }
			};

			Matrix::SMatrix<float, 2, 3> mat1 = tab1;
			Matrix::SMatrix<float, 2, 3> mat2 = tab2;

			mat1 += mat2;

			for (unsigned int i = 0; i < 6; i++)
			{
				unsigned int I = i / 3;
				unsigned int J = i % 3;

				Assert::AreEqual(7.0f, mat1(I, J));
			}

			Matrix::SMatrix<float, 2, 3> mat3 = mat1 + mat2;

			for (unsigned int i = 0; i < 6; i++)
			{
				unsigned int I = i / 3;
				unsigned int J = i % 3;

				Assert::AreEqual(mat1(I, J) + mat2(I, J), mat3(I, J));
			}
		}

		TEST_METHOD(UT07_SubstractMatrix)
		{
			const float tab1[2][3] = {
				{ 1.0f, 2.0f, 3.0f },
				{ 4.0f, 5.0f, 6.0f }
			};

			const float tab2[2][3] = {
				{ 6.0f, 5.0f, 4.0f },
				{ 3.0f, 2.0f, 1.0f }
			};

			Matrix::SMatrix<float, 2, 3> mat1 = tab1;
			Matrix::SMatrix<float, 2, 3> mat2 = tab2;

			mat1 -= mat2;

			for (unsigned int i = 0; i < 6; i++)
			{
				unsigned int I = i / 3;
				unsigned int J = i % 3;

				Assert::AreEqual(tab1[I][J] - tab2[I][J] , mat1(I, J));
			}

			Matrix::SMatrix<float, 2, 3> mat3 = mat1 - mat2;

			for (unsigned int i = 0; i < 6; i++)
			{
				unsigned int I = i / 3;
				unsigned int J = i % 3;

				Assert::AreEqual(mat1(I, J) - mat2(I, J), mat3(I, J));
			}
		}

		TEST_METHOD(UT08_MultiplyMatrix)
		{
			const float tab[2][3] = {
				{ 1.0f, 2.0f, 3.0f },
				{ 4.0f, 5.0f, 6.0f }
			};

			const float vec[3][1] = {
				{  1.0f }, 
				{ -1.0f },
				{  0.0f }
			};

			Matrixf<2, 3> mat1 = tab;
			Matrixf<3, 1> mat2 = vec;

			auto mat3 = mat1 * mat2;

			Assert::AreEqual(-1.0f, mat3(0, 0));
			Assert::AreEqual(-1.0f, mat3(1, 0));
		}
	};
}