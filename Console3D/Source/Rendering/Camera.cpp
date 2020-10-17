#include "Camera.h"

Camera::Camera() :
	m_Focal(150.0f)
{
	const float tab[3][4] = {
		m_Focal, 0.0f,    0.0f,  0.0f,
		0.0f,    m_Focal, 0.0f,  0.0f,
		0.0f,    0.0f,    1.0f,  0.0f,
	};

	m_Projection = Matrixf34(tab);
}
