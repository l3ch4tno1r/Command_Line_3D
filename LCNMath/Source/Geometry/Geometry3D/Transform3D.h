#pragma once

#include "Matrix\Stack\SqrSMatrix.h"

#include "HVector3D.h"

using namespace LCNMath::Geometry::Dim3;

using SMatrix44f    = LCNMath::StaticMatrix::Matrix<float, 4, 4>;
using SqrSMatrix44f = LCNMath::StaticMatrix::SqrMatrix<float, 4>;

struct Transform3D
{
	union
	{
		struct
		{
			float Rux, Rvx, Rwx, Tx,
				  Ruy, Rvy, Rwy, Ty,
				  Ruz, Rvz, Rwz, Tz;

			// Representation of :
			// [ Rux, Rvx, Rwx, Tx ]
			// [ Ruy, Rvy, Rwy, Ty ]
			// [ Ruz, Rvz, Rwz, Tz ]
			// [  0,   0,   0,   1 ]
		};

		SqrSMatrix44f mat;
	};

	Transform3D();

	Transform3D(const Transform3D& _tr);

	Transform3D(const SMatrix44f& _mat);

	Transform3D(const SqrSMatrix44f& _mat);
};

Transform3D operator*(const Transform3D& a, const Transform3D& b);

HVector3D operator*(const Transform3D& t, const HVector3D& v);