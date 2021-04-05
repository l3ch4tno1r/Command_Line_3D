#include "ConsoleRenderer.h"

#include "Console3D/Source/Core/Console.h"
#include "Console3D/Source/Scene/StdComponent.h"

#include <LCN_Math/Source/Geometry/Geometry.h>

#include <LCN_Collisions/Source/Collisions/CollisionAlgorithms.h>

namespace LCN::Render
{
	void Render::ConsolerRenderer::Init(size_t width, size_t height, size_t fontw, size_t fonth)
	{
		Core::Console::Get().ConstructConsole(width, height, fontw, fonth);
	}

	size_t Render::ConsolerRenderer::Width() { return Core::Console::Get().Width(); }

	size_t Render::ConsolerRenderer::Height() { return Core::Console::Get().Height(); }

	void Render::ConsolerRenderer::Render3D(const Scene& scene, Entity camera)
	{
		Core::Console& console = Core::Console::Get();

		static const char* value = " -.:*+=%#@";

		// Get camera parameters
		const Component::CameraCmp& camCmp = camera.Get<Component::CameraCmp>();
		const Render::Camera&       cam    = camCmp.Camera;

		const Transform3Df& R0ToCam  = camera.Get<Component::Transform3DCmp>().Transform;
		const Transform2Df& camToPix = cam.PixToCam().QuickInverse();

		auto view = scene.Registry().view<const Component::Transform3DCmp, const Component::InfiniteChessboardCmp>();

		console.FillRectangle(
			0, 0,
			console.Width(), console.Height(),
			[&](int i, int j)
			{
				HVector2Df pixFromCam    = camToPix * HVector2Df({ (float)i, (float)j }, 1.0f);
				HVector3Df pixDirFromCam = { {pixFromCam.x(), pixFromCam.y(), cam.Focal()}, 0.0f };
				HVector3Df pixDirFromR0  = R0ToCam * pixDirFromCam;

				Line3Df  line{ R0ToCam.TranslationBlock(), pixDirFromR0.Vector() };

				CHAR_INFO pixel;

				pixel.Char.UnicodeChar = 0;
				pixel.Attributes = Core::COLOUR::BG_BLACK;

				view.each([&](
					const Component::Transform3DCmp&        transformCmp,
					const Component::InfiniteChessboardCmp& chessboardCmp)
					{
						Vector3Df origin = transformCmp.Transform.TranslationBlock();
						Vector3Df normal = transformCmp.Transform.RotationBlock().Columns()[2];
						
						Plane3Df plane{ origin, normal };
						
						PlaneVSLine3Df result;
						
						ComputeCollision(plane, line, result);
						
						if (!result)
							return;

						const float distance = result.Coordinate();

						if (distance < camCmp.NearClip)
							return;

						float lighting = 9 * std::min(1.0f, 5.0f / distance);

						const HVector3Df& intersection = result.Result();

						// TODO : transform

						int i = (int)intersection.x() / chessboardCmp.Width;
						int j = (int)intersection.y() / chessboardCmp.Height;

						pixel.Char.UnicodeChar = std::abs(i + j) % 2 == 0 ? 0 : value[(size_t)lighting];
						pixel.Attributes = Core::COLOUR::BG_BLACK | Core::COLOUR::FG_WHITE;
					});

				return pixel;
			});

		console.Render();
	}
}