#pragma once

#include "Console3D/External/entt/include/entt.hpp"

// Design taken from The Cherno : https://github.com/TheCherno/Hazel/blob/master/Hazel/src/Hazel/Scene/Scene.h
namespace LCN
{
	class Entity;

	class Scene
	{
	public:
		Entity CreateEntity();

		void Render(Entity camera);

	private:
		entt::registry m_Registry;

		friend class Entity;
	};
}