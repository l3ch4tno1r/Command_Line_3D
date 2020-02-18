#include "stdafx.h"
#include "CppUnitTest.h"

#include "Matrix\Stack\SMatrix.h"
#include "Matrix\Stack\SqrSMatrix.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MatrixUT
{		
	TEST_CLASS(SMatrixUT)
	{
		template<Matrix::uint L, Matrix::uint C >
		using Matrixf = Matrix::SMatrix<float, L, C>;

		template<Matrix::uint LC >
		using SqrMatrixf = Matrix::SqrSMatrix<float, LC>;

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

			Assert::IsTrue(mat3 == Matrixf<2, 3>::Zero());
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

			Matrix::SMatrix<float, 2, 3> mat1 = tab;

			mat1(0, 0) = 4;

			Assert::AreEqual(4.0f, mat1(0, 0));

			const Matrix::SMatrix<float, 2, 3>& ref = mat1;

			mat1(0, 1) = -2;

			Assert::AreEqual(-2.0f, mat1(0, 1));
			Assert::AreEqual(-2.0f, ref(0, 1));

			Matrixf<2, 3> mat2;

			mat2 = mat1;

			Assert::IsTrue(mat1 == mat2);
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

		TEST_METHOD(UT09_MatrixComparison)
		{
			const float tab[2][3] = {
				{ 1.0f, 2.0f, 3.0f },
				{ 4.0f, 5.0f, 6.0f }
			};

			Matrixf<2, 3> mat1 = tab;
			Matrixf<2, 3> mat2 = tab;

			Assert::IsTrue(mat1 == mat2);
			Assert::IsFalse(mat1 != mat2);

			mat1(0, 1) = 3.0f;

			Assert::IsFalse(mat1 == mat2);
			Assert::IsTrue(mat1 != mat2);
		}

		TEST_METHOD(UT10_OperationsOnLines)
		{
			const float tab[2][3] = {
				{ 1.0f, 2.0f, 3.0f },
				{ 4.0f, 5.0f, 6.0f }
			};

			Matrixf<2, 3> mat = tab;

			mat.ScaleLine(0, 2.0f);

			Assert::AreEqual(2.0f, mat(0, 0));
			Assert::AreEqual(4.0f, mat(0, 1));
			Assert::AreEqual(6.0f, mat(0, 2));

			mat.CombineLines(1, 1.0f, 0, -2.0f);

			Assert::AreEqual( 0.0f, mat(1, 0));
			Assert::AreEqual(-3.0f, mat(1, 1));
			Assert::AreEqual(-6.0f, mat(1, 2));
		}

		TEST_METHOD(UT11_SubMatrix)
		{
			const float tab1[2][3] = {
				{ 1.0f, 2.0f, 3.0f },
				{ 4.0f, 5.0f, 6.0f }
			};

			Matrixf<2, 3> mat1 = tab1;
			Matrixf<2, 2> mat2 = mat1.SubMatrix<2, 2>(0, 0);

			Assert::AreEqual(1.0f, mat2(0, 0));
			Assert::AreEqual(2.0f, mat2(0, 1));
			Assert::AreEqual(4.0f, mat2(1, 0));
			Assert::AreEqual(5.0f, mat2(1, 1));

			Assert::ExpectException<std::out_of_range>([&mat1]() { mat1.SubMatrix<2, 2>(0, 2); });
			Assert::ExpectException<std::out_of_range>([&mat1]() { mat1.SubMatrix<2, 2>(1, 0); });
			Assert::ExpectException<std::out_of_range>([&mat1]() { mat1.SubMatrix<2, 2>(1, 2); });

			mat1.SubMatrix(mat2, 0, 1);

			Assert::AreEqual(1.0f, mat1(0, 0));
			Assert::AreEqual(1.0f, mat1(0, 1));
			Assert::AreEqual(2.0f, mat1(0, 2));
			Assert::AreEqual(4.0f, mat1(1, 0));
			Assert::AreEqual(4.0f, mat1(1, 1));
			Assert::AreEqual(5.0f, mat1(1, 2));

			Assert::ExpectException<std::out_of_range>([&mat1, &mat2]() { mat1.SubMatrix(mat2, 0, 2); });
			Assert::ExpectException<std::out_of_range>([&mat1, &mat2]() { mat1.SubMatrix(mat2, 1, 0); });
			Assert::ExpectException<std::out_of_range>([&mat1, &mat2]() { mat1.SubMatrix(mat2, 1, 2); });
		}

		TEST_METHOD(UT12_GaussElimination)
		{
			const float tab1[3][3] = {
				{ 1.0f, 0.0f,  0.0f },
				{ 0.0f, 0.0f, -1.0f },
				{ 0.0f, 1.0f,  0.0f }
			};

			Matrixf<3, 3> mat1 = tab1;

			Matrixf<3, 6> temp;

			temp.SubMatrix(mat1, 0, 0);
			temp.SubMatrix(SqrMatrixf<3>::Identity(), 0, 3);

			temp.GaussElimination();

			SqrMatrixf<3> inv = temp.SubMatrix<3, 3>(0, 3);
			SqrMatrixf<3> res = mat1 * inv;

			Assert::IsTrue(res == SqrMatrixf<3>::Identity());

			const float tab2[3][3] = {
				{  2.0f, -1.0f,  0.0f },
				{  0.0f, -1.0f,  2.0f },
				{ -1.0f,  2.0f, -1.0f}
			};

			Matrixf<3, 3> mat2 = tab2;

			float pseudodet = mat2.GaussElimination();

			Assert::IsTrue(std::abs(-4.0f - pseudodet) < 0.001);
			Assert::IsTrue(std::abs(-4.0f - pseudodet) < 0.0001);
			Assert::IsTrue(std::abs(-4.0f - pseudodet) < 0.00001);
			Assert::IsTrue(std::abs(-4.0f - pseudodet) < 0.000001);
		}
	};
}