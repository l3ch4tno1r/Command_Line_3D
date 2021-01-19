#pragma once

#include "Eigen/Core"
#include "Eigen/Geometry"

namespace LCN
{
	class Entity
	{
	public:
		Eigen::Affine2f& Frame() { return m_Frame; }
		const Eigen::Affine2f& Frame() const { return m_Frame; }

	private:
		Eigen::Affine2f m_Frame;
	};
}