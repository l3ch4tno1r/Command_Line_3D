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
		Rendering::Camera2D Camera;
		Transform2Df        PixToCam;

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
		Sprite2D     Sprite;
		Transform2Df SpriteToTexture;

		Sprite2DComponent(size_t width, size_t height)
		{
			this->Sprite.Vertices[0] = { (float)(-(int)width / 2), (float)( (int)height / 2), 1.0f };
			this->Sprite.Vertices[1] = { (float)( (int)width / 2), (float)( (int)height / 2), 1.0f };
			this->Sprite.Vertices[2] = { (float)( (int)width / 2), (float)(-(int)height / 2), 1.0f };
			this->Sprite.Vertices[3] = { (float)(-(int)width / 2), (float)(-(int)height / 2), 1.0f };

			this->Sprite.TectureCoords[0] = { 0, 0 };
			this->Sprite.TectureCoords[1] = { 1, 0 };
			this->Sprite.TectureCoords[2] = { 1, 1 };
			this->Sprite.TectureCoords[3] = { 0, 1 };

			this->Sprite.Faces[0] = { 0, 1, 3 };
			this->Sprite.Faces[1] = { 1, 2, 3 };

			this->SpriteToTexture = {
				1.0f / (float)width,  0.0f,                 0.5f,
				0.0f,                -1.0f / (float)height, 0.5f,
				0.0f,                 0.0f,                 1.0f
			};
		}
	};
}