#pragma once

#include <LCN_Math/Source/Geometry/Geometry.h>

namespace LCN
{
	class Entity
	{
	public:
		LCN::Transform2Df& Frame() { return m_Frame; }
		const LCN::Transform2Df& Frame() const { return m_Frame; }

	private:
		LCN::Transform2Df m_Frame;
	};
}