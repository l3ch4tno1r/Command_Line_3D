#pragma once

#include <vector>
#include <memory>

#include "Rendering/Camera2D.h"
#include "Entity.h"

namespace LCN
{
	class Scene
	{
	public:
		using EntityPtr = std::unique_ptr<Entity>;

		Scene();
	private:
		Camera2D m_Camera;
		std::vector<EntityPtr> m_Entities;
	};
}