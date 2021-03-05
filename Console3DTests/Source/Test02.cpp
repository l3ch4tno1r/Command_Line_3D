#include <iostream>

#include <Console3D/Source/Scene/Scene.h>
#include <Console3D/Source/Scene/Entity.h>
#include <Console3D/Source/Scene/StdComponent.h>

int main()
{
	LCN::Scene scene;

	auto view = scene.Registry().view<LCN::Component::Transform2DCmp>();

	LCN::Entity entity1 = scene.CreateEntity();

	view.each([](entt::entity entity, const LCN::Component::Transform2DCmp& transformCmp)
		{
			std::cout << transformCmp.Transform << std::endl;
		});

	std::cin.get();
}