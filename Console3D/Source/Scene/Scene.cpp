#include <cmath>

#include "Scene.h"
#include "Entity.h"

#include "StdComponent.h"

#include "Console3D/Source/Core/Console.h"

namespace LCN
{
	Entity Scene::Create2DEntity()
	{
		Entity result = { m_Registry.create(), this };

		result.Add<Component::Transform2DCmp>();

		return result;
	}

	Entity Scene::Create3DEntity()
	{
		Entity result = { m_Registry.create(), this };

		result.Add<Component::Transform3DCmp>();

		return result;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}    
}
