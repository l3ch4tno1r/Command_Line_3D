#pragma once

#include "LCN_Math/Source/Geometry/Transform.h"

class Camera
{
	using Matrixf34 = StaticMatrix<float, 3, 4>;

public:

	Camera();

	const Transform3Df& R0ToCamera() const { return m_R0ToCam; }

	const Matrixf34& Projection() const { return m_Projection; }

private:
	Transform3Df m_R0ToCam;

	float     m_Focal;
	Matrixf34 m_Projection;
};