#pragma once

#include <LCN_Math/Source/Geometry/Geometry.h>

#include "Console3D/Source/RessourceManagement/Texture.h"
#include "Console3D/Source/RessourceManagement/Sprite2D.h"

#include "Console3D/Source/Rendering/Camera2D.h"
#include "Console3D/Source/Controller/Camera2DController.h"

namespace LCN
{
	struct Transform2DComponent
	{
		Transform2Df Transform;
	};

	struct TextureComponent
	{
		Texture Texture;
	};

	struct Camera2DComponent
	{
		Camera2D     Camera;
		Transform2Df PixToCam;

		Camera2DComponent(uint32_t width, uint32_t height) :
			Camera(width, height),
			PixToCam({
				1.0f,  0.0f, (float)width / 2,
				0.0f, -1.0f, (float)height / 2,
				0.0f,  0.0f, 1.0,
			})
		{}		
	};

	struct Sprite2DComponent
	{
		Sprite2D Sprite;
	};
}