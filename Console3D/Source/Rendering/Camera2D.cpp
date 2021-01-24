#include "Camera2D.h"

namespace LCN
{
	Camera2D::Camera2D(uint32_t width, uint32_t height) :
		m_Width(width),
		m_Height(height),
		m_PixelToCenter({
			1.0f,  0.0f, (float)width / 2,
			0.0f, -1.0f, (float)height / 2,
			0.0f,  0.0f, 1.0f
		})
	{}
}
