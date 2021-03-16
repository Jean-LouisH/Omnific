#pragma once

#include "rect.hpp"
#include "transform_2d.hpp"

namespace SDL
{
	namespace Rendering2D
	{
		typedef struct Camera2D
		{
			Transform2D transform;
			Rect viewport_px;
		}Camera2D;
	}
}