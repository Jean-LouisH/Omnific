#pragma once

namespace SDL
{
	namespace Rendering2D
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