#include "Entity.h"

namespace LCN
{
	Entity::Entity(entt::entity handle, Scene* scene) :
		m_Handle(handle),
		m_Scene(scene)
	{}
}
