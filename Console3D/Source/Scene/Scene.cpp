#include <cmath>

#include "Scene.h"
#include "Entity.h"

#include "StdComponent.h"

#include "Console3D/Source/Core/Console.h"

namespace LCN
{
	//template<Component::Dimension Dim>
	Entity Scene::CreateEntity()
	{
		Entity result = { m_Registry.create(), this };

		//result.Add<Component::TransformNDCmp<Dim>>();
		result.Add<Component::Transform2DCmp>();

		return result;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}    
}
