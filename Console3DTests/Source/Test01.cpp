#include <iostream>

#include <Console3D/Source/Scene/Scene.h>
#include <Console3D/Source/Scene/Entity.h>

#define SEPARATOR(X) std::cout << "-------------- " << X << " --------------" << std::endl;

namespace LCN
{
	struct TestComponent
	{
		TestComponent()                     { std::cout << (int)this << " - Hello world - default" << std::endl; }
		TestComponent(const TestComponent&) { std::cout << (int)this << " - Hello world - copy"    << std::endl; }
		TestComponent(TestComponent&&)      { std::cout << (int)this << " - Hello world - move"    << std::endl; }

		~TestComponent() { std::cout << (int)this << " - Goodbye world" << std::endl; }

		TestComponent& operator=(TestComponent&&) { std::cout << "Move assign" << std::endl; return *this; }

		int DummyProperty = 0;
	};
}

int main()
{
	{
		LCN::Scene scene;

		LCN::Entity entity1 = scene.CreateEntity();

		entity1.Add<LCN::TestComponent>();

		scene.DestroyEntity(entity1);

		SEPARATOR("End");
	}

	std::cin.get();
}