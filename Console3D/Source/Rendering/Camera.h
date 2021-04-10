#pragma once

#include <cstdint>

#include <LCN_Math/Source/Geometry/Geometry.h>

#include <Console3D/Source/Event/KeyBoardEvent.h>
#include <Console3D/Source/Event/MouseEvent.h>

namespace LCN::Render
{
	class Camera
	{
	public:
		Camera(uint32_t width, uint32_t height, float focal = 1.0f);

		inline uint32_t Width()  const { return m_Width; }
		inline uint32_t Height() const { return m_Height; }

		inline float Focal() const { return m_Focal; }

		inline const Transform2Df& PixToCam() const { return m_PixToCam; }

	private:
		uint32_t m_Width;
		uint32_t m_Height;

		float m_Focal;

		Transform2Df m_PixToCam;
	};
}