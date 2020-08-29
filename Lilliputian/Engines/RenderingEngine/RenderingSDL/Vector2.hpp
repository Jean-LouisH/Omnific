#pragma once

namespace SDL
{
	namespace Rendering2D
	{
		typedef struct Vector2
		{
			float x;
			float y;

			Vector2() :
				x(0.0),
				y(0.0)
			{}
		}Vector2;

	}
}