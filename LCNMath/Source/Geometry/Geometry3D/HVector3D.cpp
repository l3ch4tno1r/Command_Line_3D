#include "HVector3D.h"

namespace LCNMath {
	namespace Geometry {
		namespace Dim3 {

			HVector3D::HVector3D(bool ispoint) :
				mat(1.0f)
			{
				mat(3, 0) = (ispoint ? 1.0f : 0.0f);
			}

			HVector3D::HVector3D(const HVector3D& vec) :
				mat(vec.mat)
			{}

			HVector3D::HVector3D(float _x, float _y, float _z, bool ispoint) :
				x(_x),
				y(_y),
				z(_z)
			{
				mat(3, 0) = (ispoint ? 1.0f : 0.0f);
			}

			HVector3D::HVector3D(const Vector4Df& _mat) :
				mat(_mat)
			{}

			HVector3D::HVector3D(const MatrixN1f<4>& _mat) :
				mat(_mat)
			{}

			const HVector3D& HVector3D::X()
			{
				static HVector3D x(1.0f, 0.0f, 0.0f, false);
				return x;
			}

			const HVector3D& HVector3D::Y()
			{
				static HVector3D y(0.0f, 1.0f, 0.0f, false);
				return y;
			}

			const HVector3D& HVector3D::Z()
			{
				static HVector3D z(0.0f, 0.0f, 1.0f, false);
				return z;
			}

			const HVector3D & HVector3D::Zero()
			{
				static HVector3D zero(0.0f, 0.0f, 0.0f);
				return zero;
			}

			HVector3D& HVector3D::operator=(const HVector3D& vec)
			{
				mat = vec.mat;

				return *this;
			}

			float HVector3D::Norm() const
			{
				return x * x + y * y + z * z;
			}

			void HVector3D::Normalize()
			{
				float norm = Norm();

				x /= norm;
				y /= norm;
				z /= norm;
			}

			HVector3D operator^(const HVector3D& vec1, const HVector3D& vec2)
			{
				static HVector3D result(false);

				result.x = vec1.y * vec2.z - vec1.z * vec2.y;
				result.y = vec1.z * vec2.x - vec1.x * vec2.z;
				result.z = vec1.x * vec2.y - vec1.y * vec2.x;

				return result;
			}

			float operator|(const HVector3D& vec1, const HVector3D& vec2)
			{
				// TODO : Quick fix
				//return vec1.mat | vec2.mat;
				return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
			}

			HVector3D operator+(const HVector3D& a, const HVector3D& b)
			{
				HVector3D result = a.mat + b.mat;

				result.s = 1.0f;

				return result;
			}

			HVector3D operator-(const HVector3D& a, const HVector3D& b)
			{
				HVector3D result = a.mat - b.mat;

				result.s = 0.0f;

				return result;
			}

			HVector3D operator*(float t, const HVector3D& vec)
			{
				HVector3D result(vec);

				for (uint i = 0; i < 3; i++)
					result.mat(i, 0) *= t;

				return result;
			}
		}
	}
}