#pragma once

#include "Geometry/Geometry.h"

#include "Texture.h"
#include "Sprite2D.h"
#include "Model3D.h"

#include "Camera.h"
#include "Camera2DController.h"

namespace LCN::Component
{
	enum class Dimension : short
	{
		_2 = 2,
		_3 = 3
	};

	template<Dimension Dim>
	struct TransformNDCmp
	{
		Transform<float, static_cast<size_t>(Dim)> Transform;
	};

	using Transform2DCmp = TransformNDCmp<Dimension::_2>;
	using Transform3DCmp = TransformNDCmp<Dimension::_3>;

	struct TextureCmp
	{
		Ressource::Texture Texture;
	};

	// Geometrical primitives
	struct InfiniteChessboardCmp
	{
		size_t Width;
		size_t Height;

		InfiniteChessboardCmp(size_t width, size_t height) :
			Width(width),
			Height(height)
		{}
	};

	struct SphereCmp
	{
		float Radius;

		SphereCmp(float radius) :
			Radius(radius)
		{}
	};

	struct CubeCmp
	{
		float EdgeLength;

		CubeCmp(float edgeLength) :
			EdgeLength(edgeLength)
		{}
	};

	struct CameraCmp
	{
		Render::Camera Camera;

		float NearClip;
		float FarClip;
	
		CameraCmp(uint32_t width, uint32_t height, float focal = 1.0f, float nearClip = 0.1f, float farClip = 1000000.0f) :
			Camera(width, height, focal),
			NearClip(nearClip),
			FarClip(farClip)
		{}
	};

	struct Sprite2DCmp
	{
		Ressource::Sprite2D Sprite;
		Transform2Df        SpriteToTexture;

		Sprite2DCmp(size_t width, size_t height)
		{
			this->Sprite.Vertices[0] = { (float)(-(int)width / 2), (float)((int)height  / 2), 1.0f };
			this->Sprite.Vertices[1] = { (float)( (int)width / 2), (float)((int)height  / 2), 1.0f };
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

	struct FrameCmp
	{
		float Scale;
	};

	struct HierarchyCmp
	{
		Transform2Df Offset;
		Entity       Parent;

		HierarchyCmp(Entity parent) : Parent(parent)
		{}
	};

	struct AnimationCmp
	{
		int a;
	};

	struct Mesh3DCmp
	{
		Ressource::Model3D m_Mesh;
	};
}