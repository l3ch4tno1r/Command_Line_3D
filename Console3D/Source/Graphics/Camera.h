#pragma once

#include <../LCN_Math/Source/Geometry/Geometry3D/Transform3D.h>

class Camera
{
	using Matrixf34 = LCNMath::Matrix::StaticMatrix::Matrix<float, 3, 4>;

private:
	Transform3D<float> m_R0ToCam;

	float     m_Focal;
	Matrixf34 m_Projection;

public:

	Camera();

	inline const Transform3D<float>& R0ToCamera() const
	{
		return m_R0ToCam;
	}

	inline const Matrixf34& Projection() const
	{
		return m_Projection;
	}
};