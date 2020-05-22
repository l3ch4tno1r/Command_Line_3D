#pragma once

#include "Geometry\VectorND.h"

namespace LCNMath {
	namespace Geometry {
		namespace Dim3 {

			template<typename T>
			using Vector4D = LCNMath::Geometry::VectorND<T, 4>;

			using LCNMath::Geometry::MatrixN1;

			template<typename T>
			struct HVector3D
			{
				union
				{
					struct
					{
						T x;
						T y;
						T z;
						T s;
					};

					Vector4D<T> mat;
				};

				HVector3D(bool ispoint) :
					mat(1.0f)
				{
					mat(3, 0) = (ispoint ? 1.0f : 0.0f);
				}

				HVector3D(float _x, float _y, float _z, bool ispoint = true) :
					x(_x),
					y(_y),
					z(_z)
				{
					mat(3, 0) = (ispoint ? 1.0f : 0.0f);
				}

				HVector3D(const HVector3D& vec) :
					mat(vec.mat)
				{}

				HVector3D(const Vector4D<T>& _mat) :
					mat(_mat)
				{}

				HVector3D(const MatrixN1<T, 4>& _mat) :
					mat(_mat)
				{}

				static const HVector3D& X()
				{
					static HVector3D x(1.0f, 0.0f, 0.0f, false);
					return x;
				}

				static const HVector3D& Y()
				{
					static HVector3D y(0.0f, 1.0f, 0.0f, false);
					return y;
				}

				static const HVector3D& Z()
				{
					static HVector3D z(0.0f, 0.0f, 1.0f, false);
					return z;
				}

				static const HVector3D& Zero()
				{
					static HVector3D zero(0.0f, 0.0f, 0.0f);
					return zero;
				}

				HVector3D& operator=(const HVector3D& vec)
				{
					mat = vec.mat;

					return *this;
				}

				T Norm() const
				{
					return x * x + y * y + z * z;
				}

				void Normalize()
				{
					float norm = std::sqrt(Norm());

					x /= norm;
					y /= norm;
					z /= norm;
				}
			};

			template<typename T>
			HVector3D<T> operator^(const HVector3D<T>& vec1, const HVector3D<T>& vec2)
			{
				static HVector3D<T> result(false);

				result.x = vec1.y * vec2.z - vec1.z * vec2.y;
				result.y = vec1.z * vec2.x - vec1.x * vec2.z;
				result.z = vec1.x * vec2.y - vec1.y * vec2.x;

				return result;
			}

			template<typename T>
			T operator|(const HVector3D<T>& vec1, const HVector3D<T>& vec2)
			{
				// TODO : Quick fix
				//return vec1.mat | vec2.mat;
				return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
			}

			template<typename T>
			HVector3D<T> operator+(const HVector3D<T>& a, const HVector3D<T>& b)
			{
				HVector3D<T> result = a.mat + b.mat;

				result.s = 1.0f;

				return result;
			}

			template<typename T>
			HVector3D<T> operator-(const HVector3D<T>& a, const HVector3D<T>& b)
			{
				HVector3D<T> result = a.mat - b.mat;

				result.s = 0.0f;

				return result;
			}

			template<typename T>
			HVector3D<T> operator*(float t, const HVector3D<T>& vec)
			{
				HVector3D<T> result(vec);

				for (uint i = 0; i < 3; i++)
					result.mat(i, 0) *= t;

				return result;
			}

			template<typename T>
			HVector3D<T> operator/(const HVector3D<T>& vec, float t)
			{
				HVector3D<T> result(vec);

				for (uint i = 0; i < 3; i++)
					result.mat(i, 0) /= t;

				return result;
			}
		}
	}
}