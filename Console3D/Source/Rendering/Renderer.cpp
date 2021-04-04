#include "Renderer.h"

#include "Console3D/Source/Core/Console.h"
#include "Console3D/Source/Scene/StdComponent.h"

#include <LCN_Math/Source/Geometry/Geometry.h>

#include <LCN_Collisions/Source/Collisions/CollisionAlgorithms.h>

namespace LCN::Render
{
	void Render::Renderer<Core::Console>::Init(size_t width, size_t height, size_t fontw, size_t fonth)
	{
		Core::Console::Get().ConstructConsole(width, height, fontw, fonth);
	}

	size_t Render::Renderer<Core::Console>::Width() { return Core::Console::Get().Width(); }

	size_t Render::Renderer<Core::Console>::Height() { return Core::Console::Get().Height(); }

	void Render::Renderer<Core::Console>::Render3D(const Scene& scene, Entity camera)
	{
		Core::Console& console = Core::Console::Get();

		const Transform3Df& R0ToCam = camera.Get<Component::Transform3DCmp>().Transform;

		// Get camera intrinsec parameters
		const auto& camCmp = camera.Get<Component::Camera2DCmp>();

		auto view = scene.Registry().view<const Component::Transform3DCmp, const Component::InfiniteChessboardCmp>();

		console.FillRectangle(
			0, 0,
			console.Width(), console.Height(),
			[&](int i, int j)
			{
				Line3Df line{ {0, 0}, {(float)i, (float)j } };

				CHAR_INFO result;

				result.Char.UnicodeChar = 0;
				result.Attributes = Core::COLOUR::BG_BLACK;

				view.each([&](
					const Component::Transform3DCmp&        transform,
					const Component::InfiniteChessboardCmp& chessboard)
					{

					});

				return result;
			});

		console.Render();
	}
}