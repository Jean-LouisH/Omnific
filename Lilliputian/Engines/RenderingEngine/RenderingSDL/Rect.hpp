#pragma once

namespace Rendering2D
{
	namespace SDL
	{
		typedef struct Rect
		{
			double width;
			double height;

			Rect() :
				width(0.0),
				height(0.0)
			{}
		}Rect;
	}
}
