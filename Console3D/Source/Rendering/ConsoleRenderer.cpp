#include "ConsoleRenderer.h"

#include "Console3D/Source/Core/Console.h"
#include "Console3D/Source/Scene/StdComponent.h"

#include <LCN_Math/Source/Geometry/Geometry.h>
#include <LCN_Math/Source/Utilities/Angles.h>

#include <LCN_Collisions/Source/Collisions/CollisionAlgorithms.h>

#include <Utilities/Source/Instrumentor.h>

//#define DEBUG_APP

#ifdef DEBUG_APP
#define OFFSET 7
#else
#define OFFSET 0
#endif

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

		// Grab views
		auto chessboardView     = scene.Registry().view<const Component::Transform3DCmp, const Component::InfiniteChessboardCmp>();
		auto texturedSphereView = scene.Registry().view<const Component::Transform3DCmp, const Component::SphereCmp, const Component::TextureCmp>();
		auto texturedCubeView   = scene.Registry().view<const Component::Transform3DCmp, const Component::CubeCmp, const Component::TextureCmp>();

		console.FillScreen([&](int i, int j)
		{
			CHAR_INFO pixel;

			pixel.Char.UnicodeChar = 0;
			pixel.Attributes = Core::COLOUR::BG_DARK_GREY;

			HVector2Df pixFromCam    = camToPix * HVector2Df({ (float)i, (float)j }, 1.0f);
			HVector3Df pixDirFromCam = { {pixFromCam.x(), pixFromCam.y(), cam.Focal()}, 0.0f };
			HVector3Df pixDirFromR0  = R0ToCam * pixDirFromCam;

			Line3Df lineFromR0{ R0ToCam.TranslationBlock(), pixDirFromR0.Vector() };

			// Render infinite chessboard
			chessboardView.each([&](
				const Component::Transform3DCmp&        transformCmp,
				const Component::InfiniteChessboardCmp& chessboardCmp)
			{
				Vector3Df origin = transformCmp.Transform.TranslationBlock();
				Vector3Df normal = transformCmp.Transform.RotationBlock().Columns()[2];
				
				Hyperplane3Df plane{ origin, normal };
					
				HyperplaneVSLine3Df result;
					
				ComputeCollision(plane, lineFromR0, result);
					
				if (!result)
					return;

				const float distance = result.Coordinate();

				if (distance < camCmp.NearClip)
					return;

				float lighting = 9 * std::min(1.0f, 10.0f / distance);

				const HVector3Df& intersection = result.Result();

				// TODO : transform

				float x = intersection.x();
				float y = intersection.y();

				int i = std::floor(x / chessboardCmp.Width);
				int j = std::floor(y / chessboardCmp.Height);

				pixel.Char.UnicodeChar = std::abs(i + j) % 2 == 0 ? 0 : value[(size_t)lighting];
				pixel.Attributes = Core::COLOUR::BG_BLACK | Core::COLOUR::FG_WHITE;
			});

			// Render textured sphere
			texturedSphereView.each([&](
				const Component::Transform3DCmp& transformCmp,
				const Component::SphereCmp&      sphereCmp,
				const Component::TextureCmp&     textureCmp)
			{
				Vector3Df center = transformCmp.Transform.TranslationBlock();

				Sphere3Df sphere{ center, sphereCmp.Radius };

				SphereVSLine3Df result;

				ComputeCollision(sphere, lineFromR0, result);

				if (!result)
					return;

				const auto& firstInter = result[0];

				if (firstInter.Distance < camCmp.NearClip)
					return;

				HVector3Df pointFromSphere = transformCmp.Transform.QuickInverse() * firstInter.Point;

				if (std::abs(pointFromSphere.x()) < 0.001 && std::abs(pointFromSphere.y()) < 0.001)
					return;

				float rho   = pointFromSphere.Norm();
				float theta = std::acos(pointFromSphere.z() / rho);
				float phi   = std::atan2(pointFromSphere.y(), pointFromSphere.x());

				int tx = textureCmp.Texture.Width() * (1 - (phi + PI) / (2 * PI));
				int ty = textureCmp.Texture.Height() * theta / PI;

				Ressource::TexelGreyScale texel = textureCmp.Texture(tx, ty);

				uint8_t greyscale = 9 * (uint8_t)texel.GreyScale / 255;

				pixel.Char.UnicodeChar = value[greyscale];
				pixel.Attributes = Core::COLOUR::FG_WHITE | Core::COLOUR::BG_BLACK;
			});

			// Render textured cube
			texturedCubeView.each([&](
				const Component::Transform3DCmp& transformCmp,
				const Component::CubeCmp&        cubeCmp,
				const Component::TextureCmp&     textureCmp)
			{
				Transform3Df cubeToR0 = transformCmp.Transform.QuickInverse();

				HVector3Df originFromCube    = cubeToR0 * lineFromR0.Origin();
				HVector3Df directionFromCube = cubeToR0 * lineFromR0.Direction();

				Line3Df lineFromCube{ originFromCube.Vector(), directionFromCube.Vector() };

				AABB3Df aabb{ { 0, 0, 0 }, { cubeCmp.EdgeLength, cubeCmp.EdgeLength, cubeCmp.EdgeLength } };

				AABBVSLine3Df result;

				ComputeCollision(aabb, lineFromCube, result);

				if (!result)
					return;

				if (result[0].Distance < camCmp.NearClip)
					return;

				pixel.Attributes = Core::COLOUR::BG_BLUE;
			});

			return pixel;
		});

		console.Render();
	}
}