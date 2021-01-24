#pragma once

#include "../Rendering/Camera2D.h"
#include "../Scene/Entity.h"

namespace LCN
{
	class Camera2DController : public Entity
	{
	public:
		Camera2DController(Camera2D& camref) :
			m_Camera(camref)
		{}

	private:
		Camera2D& m_Camera;
	};
}