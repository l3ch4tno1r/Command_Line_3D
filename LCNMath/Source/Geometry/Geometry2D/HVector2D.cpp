#include "HVector2D.h"

namespace LCNMath {
	namespace Geometry {
		namespace Dim2 {

			HVector2D::HVector2D() :
				mat(1.0f)
			{}

			HVector2D::HVector2D(float _x, float _y) :
				x(_x),
				y(_y)
			{
				mat(2, 0) = 1.0f;
			}

			HVector2D::HVector2D(const HVector2D& vec) :
				mat(vec.mat)
			{}

			HVector2D::HVector2D(const Vector3Df& _mat) :
				mat(_mat)
			{}

			HVector2D::HVector2D(const MatrixN1f<3>& _mat) :
				mat(_mat)
			{}

			void HVector2D::Homogenize()
			{
				x /= s;
				y /= s;
				s = 1.0f;
			}

			HVector2D& HVector2D::operator=(const HVector2D& vec)
			{
				mat = vec.mat;

				return *this;
			}

			float operator|(const HVector2D& a, const HVector2D& b)
			{
				return a.mat | b.mat;
			}

			HVector2D operator+(const HVector2D& a, const HVector2D& b)
			{
				HVector2D result = a.mat + b.mat;

				result.s = 1.0f;

				return result;
			}

			HVector2D operator-(const HVector2D & a, const HVector2D & b)
			{
				HVector2D result = a.mat - b.mat;

				result.s = 0.0f;

				return result;
			}

			HVector2D operator*(float t, const HVector2D & vec)
			{
				HVector2D result(vec);

				for (uint i = 0; i < 2; i++)
					result.mat(i, 0) *= t;

				return result;
			}
		}
	}
}


