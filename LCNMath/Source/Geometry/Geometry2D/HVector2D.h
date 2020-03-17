#pragma once

#include "Geometry\VectorND.h"

namespace LCNMath {
	namespace Geometry {
		namespace Dim2{

			using Vector3Df = LCNMath::Geometry::VectorND<3>;
			using LCNMath::Geometry::MatrixN1f;

			struct HVector2D
			{
				union
				{
					struct
					{
						float x;
						float y;
						float s;
					};

					Vector3Df mat;
				};

				HVector2D();

				HVector2D(float _x, float _y);

				HVector2D(const HVector2D& vec);

				HVector2D(const Vector3Df& _mat);

				HVector2D(const MatrixN1f<3>& _mat);

				inline float PX() const
				{
					return x / s;
				}

				inline float PY() const
				{
					return y / s;
				}

				void Homogenize();

				HVector2D& operator=(const HVector2D& vec);
			};

			float operator|(const HVector2D& a, const HVector2D& b);

			HVector2D operator+(const HVector2D& a, const HVector2D& b);

			HVector2D operator-(const HVector2D& a, const HVector2D& b);

			HVector2D operator*(float t, const HVector2D& vec);
		}
	}
}