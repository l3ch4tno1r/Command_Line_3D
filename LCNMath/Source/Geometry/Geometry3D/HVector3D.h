#pragma once

#include "Geometry\VectorND.h"

namespace LCNMath {
	namespace Geometry {
		namespace Dim3 {

			using Vector4Df = LCNMath::Geometry::VectorND<4>;
			using LCNMath::Geometry::MatrixN1f;

			struct HVector3D
			{
				union
				{
					struct
					{
						float x;
						float y;
						float z;
						float s;
					};

					Vector4Df mat;
				};

				HVector3D(bool ispoint);

				HVector3D(float _x, float _y, float _z, bool ispoint = true);

				HVector3D(const HVector3D& vec);

				HVector3D(const Vector4Df& _mat);

				HVector3D(const MatrixN1f<4>& _mat);

				static const HVector3D& X();

				static const HVector3D& Y();

				static const HVector3D& Z();

				static const HVector3D& Zero();

				HVector3D& operator=(const HVector3D& vec);

				float Norm() const;

				void Normalize();
			};

			HVector3D operator^(const HVector3D& vec1, const HVector3D& vec2);

			float operator|(const HVector3D& vec1, const HVector3D& vec2);

			HVector3D operator+(const HVector3D& a, const HVector3D& b);

			HVector3D operator-(const HVector3D& a, const HVector3D& b);

			HVector3D operator*(float t, const HVector3D& vec);

			HVector3D operator/(const HVector3D& vec, float t);
		}
	}
}