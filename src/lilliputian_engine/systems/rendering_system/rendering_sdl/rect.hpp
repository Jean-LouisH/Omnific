#pragma once

namespace SDL
{
	namespace Rendering2D
	{
		typedef struct Rect
		{
			float width;
			float height;

			Rect() :
				width(0.0),
				height(0.0)
			{}
		}Rect;
	}
}
