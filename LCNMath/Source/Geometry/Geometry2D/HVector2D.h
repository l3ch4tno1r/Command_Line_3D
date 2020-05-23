#pragma once

#include "Geometry\VectorND.h"

namespace LCNMath {
	namespace Geometry {
		namespace Dim2{

			template<typename T>
			using Vector3D = LCNMath::Geometry::VectorND<T, 3>;

			using LCNMath::Geometry::MatrixN1;

			template<typename T>
			struct HVector2D
			{
				union
				{
					struct
					{
						T x;
						T y;
						T s;
					};

					Vector3D<T> mat;
				};

				HVector2D() :
					mat(1.0f)
				{}

				HVector2D(float _x, float _y) :
					x(_x),
					y(_y)
				{
					mat(2, 0) = 1.0f;
				}

				HVector2D(const HVector2D& vec) :
					mat(vec.mat)
				{}

				HVector2D(const Vector3D<T>& _mat) :
					mat(_mat)
				{}

				HVector2D(const MatrixN1<T, 3>& _mat) :
					mat(_mat)
				{}

				inline float PX() const
				{
					return x / s;
				}

				inline float PY() const
				{
					return y / s;
				}

				void Homogenize()
				{
					x /= s;
					y /= s;
					s = 1.0f;
				}

				HVector2D& operator=(const HVector2D& vec)
				{
					mat = vec.mat;

					return *this;
				}
			};

			template<typename T>
			T operator|(const HVector2D<T>& a, const HVector2D<T>& b)
			{
				// TODO : Quick fix
				//return a.mat | b.mat;
				return a.x * b.x + a.y * b.y;
			}

			template<typename T>
			HVector2D<T> operator+(const HVector2D<T>& a, const HVector2D<T>& b)
			{
				HVector2D<T> result = a.mat + b.mat;

				result.s = 1.0f;

				return result;
			}

			template<typename T>
			HVector2D<T> operator-(const HVector2D<T>& a, const HVector2D<T>& b)
			{
				HVector2D<T> result = a.mat - b.mat;

				result.s = 0.0f;

				return result;
			}

			template<typename T>
			HVector2D<T> operator*(float t, const HVector2D<T>& vec)
			{
				HVector2D<T> result(vec);

				for (uint i = 0; i < 2; i++)
					result.mat(i, 0) *= t;

				return result;
			}

			template<typename T>
			HVector2D<T> operator/(const HVector2D<T>& vec, float t)
			{
				HVector2D<T> result(vec);

				for (uint i = 0; i < 2; i++)
					result.mat(i, 0) /= t;

				return result;
			}
		}
	}
}