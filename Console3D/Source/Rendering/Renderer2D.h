#pragma once

#include "Console3D/Source/Scene/Scene.h"
#include "Console3D/Source/Scene/Entity.h"
#include "Console3D/Source/Core/Console.h"

namespace LCN::Render
{
	struct ViewPort
	{
		int TLx, TLy;
		int BRx, BRy;
	};

	class Renderer2D
	{
	public:
		static void Render(
			Scene& scene,
			Entity cameraEntity,
			const ViewPort& viewPort = {0, 0, (int)Core::Console::Get().Width(), (int)Core::Console::Get().Height() });

		static void RenderParallel(
			Scene& scene,
			Entity cameraEntity,
			const ViewPort& viewPort = { 0, 0, (int)Core::Console::Get().Width(), (int)Core::Console::Get().Height() });
	};
}