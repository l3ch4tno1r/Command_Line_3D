#pragma once

#include "Matrix\Stack\SqrSMatrix.h"

#define PI 3.14159265
#define TORAD(A) (PI * A) / 180

using SqrSMatrix33f = Matrix::StaticMatrix::SqrMatrix<float, 3>;

union Transform2D
{
	struct
	{
		float Rux, Rvx, Tx, Ruy, Rvy, Ty;

		// Representation of :
		// [ Rux, Rvx, Tx ]
		// [ Ruy, Rvy, Ty ]
		// [  0,   0,   1 ]
;	};

	SqrSMatrix33f mat;

	Transform2D();

	Transform2D(float x, float y, float a);

	void SetRotationAngle(float a);

	void SetTranslation(float x, float y);
};