#include "stdafx.h"
#include "CppUnitTest.h"
#include "SMatrix.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{		
	TEST_CLASS(MatrixUT)
	{
	public:		
		TEST_METHOD(UT01_DefaultConstructor)
		{
			Matrix::SMatrix<float, 3, 4> mat1;
			Matrix::SMatrix<float, 8, 6> mat2;

			Assert::AreEqual(3u, mat1.Lines());
			Assert::AreEqual(4u, mat1.Columns());

			Assert::AreEqual(8u, mat2.Lines());
			Assert::AreEqual(6u, mat2.Columns());
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

			Assert::AreEqual(1.0f, mat1(0, 0));
			Assert::AreEqual(2.0f, mat1(0, 1));
			Assert::AreEqual(3.0f, mat1(0, 2));
			Assert::AreEqual(4.0f, mat1(1, 0));
			Assert::AreEqual(5.0f, mat1(1, 1));
			Assert::AreEqual(6.0f, mat1(1, 2));

			Matrix::SMatrix<float, 2, 3> mat2 = mat1;

			Assert::AreEqual(2u, mat2.Lines());
			Assert::AreEqual(3u, mat2.Columns());

			Assert::AreEqual(1.0f, mat2(0, 0));
			Assert::AreEqual(2.0f, mat2(0, 1));
			Assert::AreEqual(3.0f, mat2(0, 2));
			Assert::AreEqual(4.0f, mat2(1, 0));
			Assert::AreEqual(5.0f, mat2(1, 1));
			Assert::AreEqual(6.0f, mat2(1, 2));
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

		TEST_METHOD(UT04_DataAccessWriting)
		{
			const float tab[2][3] = {
				{ 1.0f, 2.0f, 3.0f },
			{ 4.0f, 5.0f, 6.0f }
			};

			Matrix::SMatrix<float, 2, 3> mat = tab;

			mat(0, 0) = 4;

			Assert::AreEqual(4.0f, mat(0, 0));
		}
	};
}