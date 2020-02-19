#pragma once

#include "Matrix\Stack\SqrSMatrix.h"

using SqrSMatrix44f = Matrix::StaticMatrix::SqrMatrix<float, 4>;

union Transform3D
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

	Transform3D();

	Transform3D(const SqrSMatrix44f& _mat);
};