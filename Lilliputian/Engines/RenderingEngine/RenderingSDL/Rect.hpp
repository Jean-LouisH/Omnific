#pragma once

namespace SDL
{
	namespace Rendering2D
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
