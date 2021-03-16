#pragma once

#include "vector2.hpp"

namespace SDL
{
	namespace Rendering2D
	{
		typedef struct Transform2D
		{
			Vector2 position_px;
			float rotation_rad;
			Vector2 scale;

			Transform2D() :
				rotation_rad(0.0)
			{
				this->scale.x = 1;
				this->scale.y = 1;
			}
		}Transform2D;
	}
}
