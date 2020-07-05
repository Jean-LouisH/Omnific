#pragma once

namespace Rendering2D
{
	namespace SDL
	{
		typedef struct Vector2
		{
			double x;
			double y;

			Vector2() :
				x(0.0),
				y(0.0)
			{}
		}Vector2;

	}
}