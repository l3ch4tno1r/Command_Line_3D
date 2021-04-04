#pragma once

#include "Console3D/Source/Scene/Scene.h"
#include "Console3D/Source/Scene/Entity.h"

namespace LCN::Render
{
	template<class Device>
	class Renderer
	{
	public:
		static void Init(size_t width, size_t height, size_t fontw, size_t fonth);

		static void Render3D(const Scene& scene, const Entity camera);
	};
}