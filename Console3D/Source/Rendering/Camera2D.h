#pragma once

#include <cstdint>

#include <LCN_Math/Source/Geometry/Geometry.h>

#include <Console3D/Source/Event/KeyBoardEvent.h>
#include <Console3D/Source/Event/MouseEvent.h>

namespace LCN::Rendering
{
	class Camera2D
	{
	public:
		Camera2D(uint32_t width, uint32_t height);

		uint32_t Width()  const { return m_Width; }
		uint32_t Height() const { return m_Height; }

	private:
		uint32_t m_Width;
		uint32_t m_Height;

		Transform2Df m_PixelToCenter;
	};
}