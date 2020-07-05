#pragma once

#include "Vector2.hpp"

namespace Rendering2D
{
	namespace SDL
	{
		typedef struct Transform2D
		{
			Vector2 position_px;
			double rotation_rad;
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
