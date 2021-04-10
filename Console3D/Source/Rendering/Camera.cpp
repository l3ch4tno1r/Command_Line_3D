#include "Camera.h"

namespace LCN::Render
{
	Camera::Camera(uint32_t width, uint32_t height, float focal) :
		m_Width(width),
		m_Height(height),
		m_Focal(focal),
		m_PixToCam({
			1.0f,  0.0f, (float)width / 2,
			0.0f, -1.0f, (float)height / 2,
			0.0f,  0.0f, 1.0f
		})
	{}
}
