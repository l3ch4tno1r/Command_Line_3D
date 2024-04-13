#pragma once

#include <LCN_Math/Source/Geometry/Geometry.h>

namespace LCN::Ressource
{
	struct Sprite2D
	{
		HVector2Df Vertices[4];
		Vector2Df  TectureCoords[4];

		struct Triangle
		{
			union
			{
				struct
				{
					size_t v1, v2, v3;
				};

				size_t v[3];
			};
		};

		Triangle Faces[2];
	};
}