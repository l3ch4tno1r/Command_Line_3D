#pragma once

#include "Camera2D.h"

#include "Eigen/Core"
#include "Eigen/Geometry"

namespace LCN
{
	class Camera2DController
	{
	public:
		Camera2DController(Camera2D& camref) :
			m_Camera(camref)
		{}

	private:
		Camera2D& m_Camera;
		Eigen::Affine2f m_CameraFrame;
	};
}