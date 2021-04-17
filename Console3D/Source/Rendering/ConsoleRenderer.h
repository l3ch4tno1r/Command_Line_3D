#pragma once

#include "Renderer.h"

namespace LCN::Render
{
	class ConsolerRenderer
	{
	public:
		static void Init(size_t width, size_t height, size_t fontw, size_t fonth);

		static void Render3D(const Scene& scene, Entity camera);

		static size_t Width();

		static size_t Height();
	};
}