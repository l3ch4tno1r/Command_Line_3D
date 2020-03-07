#pragma once

#include "Geometry\VectorND.h"

using Vector3Df = VectorND<3>;

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
};

float operator|(const HVector2D& a, const HVector2D& b);

HVector2D operator+(const HVector2D& a, const HVector2D& b);

HVector2D operator-(const HVector2D& a, const HVector2D& b);

HVector2D operator*(float t, const HVector2D& vec);