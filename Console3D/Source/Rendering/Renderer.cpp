#include "Renderer.h"

#include "Core/Console.h"

namespace LCN::Render
{
	template<>
	void Render::Renderer<Core::Console>::Init(size_t width, size_t height, size_t fontw, size_t fonth)
	{
		Core::Console::Get().ConstructConsole(width, height, fontw, fonth);
	}

	template<>
	void Render::Renderer<Core::Console>::Render3D(const Scene& scene, const Entity camera)
	{
		auto& console = Core::Console::Get();


	}
}