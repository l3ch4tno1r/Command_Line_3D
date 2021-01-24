#pragma once

#include <vector>
#include <memory>

#include "../Rendering/Camera2D.h"
#include "Entity.h"

namespace LCN
{
	class Scene
	{
	public:
		using EntityPtr = std::shared_ptr<Entity>;

		Scene(uint32_t w, uint32_t h);

		template<class DerivedEntity, class...Args>
		DerivedEntity& CreateEntity(Args&& ...args)
		{
			m_Entities.emplace_back(std::make_shared<DerivedEntity>(std::forward<Args>(args)...));
		}

	private:
		Camera2D m_Camera;
		std::vector<EntityPtr> m_Entities;
	};
}