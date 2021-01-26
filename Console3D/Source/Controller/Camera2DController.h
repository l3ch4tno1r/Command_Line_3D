#pragma once

#include <Utilities/Source/DesignPatterns/SignalSlot.h>

#include "../Rendering/Camera2D.h"
#include "../Scene/Entity.h"

namespace LCN
{
	class Camera2DController : public Entity
	{
	public:
		Camera2DController(const Camera2D& camref) :
			m_Camera(camref)
		{}


	private:
		const Camera2D& m_Camera;
	};
}