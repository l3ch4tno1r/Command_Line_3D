#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <LCN_Math/Source/Matrix/Matrix.h>

namespace MatrixUT
{
	TEST_CLASS(StaticMatrixUT)
	{
	public:
		TEST_METHOD(UT01_DefaultConstructor)
		{
			Matrix<float, 3, 4> mat;

			Assert::AreEqual(3u, mat.Line());
			Assert::AreEqual(4u, mat.Column());
		}

		TEST_METHOD(UT02_InitializerListConstructor)
		{
			Matrix<float, 3, 3> mat = {
				1, 2, 3,
				4, 5, 6,
				7, 8, 9
			};

			for (size_t I = 0; I < 9; ++I)
			{
				size_t i = I / 3;
				size_t j = I % 3;

				Assert::AreEqual((float)(I + 1), mat(i, j));
			}
		}

		TEST_METHOD(UT03_EvaluationConstructor)
		{
			Matrix<float, 3, 3> mat1 = {
				1, 2, 3,
				4, 5, 6,
				7, 8, 9
			};

			Matrix<float, 3, 3> mat2 = mat1;

			for (size_t i = 0; i < mat2.Line(); i++)
				for (size_t j = 0; j < mat2.Column(); j++)
					Assert::AreEqual(mat1(i, j), mat2(i, j));
		}

		TEST_METHOD(UT04_MatrixAddition)
		{
			Matrix<float, 3, 1> vec1 = { 1, 2, 3 };
			Matrix<float, 3, 1> vec2 = { 4, 5, 6 };

			Matrix<float, 3, 1> sum = vec1 + vec2;

			Assert::AreEqual(5.0f, sum(0, 0));
			Assert::AreEqual(7.0f, sum(1, 0));
			Assert::AreEqual(9.0f, sum(2, 0));

			vec1 = vec1 + vec2;

			Assert::AreEqual(5.0f, vec1(0, 0));
			Assert::AreEqual(7.0f, vec1(1, 0));
			Assert::AreEqual(9.0f, vec1(2, 0));

			vec1 += vec2;

			Assert::AreEqual( 9.0f, vec1(0, 0));
			Assert::AreEqual(12.0f, vec1(1, 0));
			Assert::AreEqual(15.0f, vec1(2, 0));
		}
	};
}