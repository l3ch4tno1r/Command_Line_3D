#pragma once

#include "Console3D/External/entt/include/entt.hpp"
#include "Scene.h"

// Design taken from The Cherno : https://github.com/TheCherno/Hazel/blob/master/Hazel/src/Hazel/Scene/Entity.h
namespace LCN
{
	class Entity
	{
	public:
		Entity() = default;

		Entity(entt::entity handle, Scene* scene) :
			m_Handle(handle),
			m_Scene(scene)
		{}

		template<class Component, class ...Args>
		Component& Add(Args&& ...args)
		{
			// TODO : check stuff here
			return m_Scene->m_Registry.emplace<Component>(m_Handle, std::forward<Args>(args)...);
		}

		template<class Component>
		Component& Get()
		{
			// TODO : check stuff here too
			return m_Scene->m_Registry.get<Component>(m_Handle);
		}

		template<class Component>
		bool Has() const
		{
			return m_Scene->m_Registry.has<Component>(m_Handle);
		}

		operator entt::entity() { return m_Handle; }

	private:
		entt::entity m_Handle{entt::null};
		Scene* m_Scene;

		friend Scene;
	};
}