#pragma once

#include <vector>
#include <memory>

#include "Entity.h"

namespace LCN
{
	class Scene
	{
	public:
		using EntityPtr = std::shared_ptr<Entity>;

		Scene() = default;

		template<class DerivedEntity, class...Args>
		DerivedEntity& CreateEntity(Args&& ...args)
		{
			m_Entities.emplace_back(
				std::make_shared<DerivedEntity>(
					std::forward<Args>(args)...));
		}

	private:
		std::vector<EntityPtr> m_Entities;
	};
}