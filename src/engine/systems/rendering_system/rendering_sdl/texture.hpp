#pragma once

#include <SDL_video.h>
#include "rect.hpp"

namespace SDL
{
	namespace Rendering2D
	{
		typedef struct Texture
		{
			SDL_Texture* data;
			Rect pixels;
		}Texture;
	}
}