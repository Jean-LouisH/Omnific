#pragma once

#include <SDL_video.h>
#include "Rect.hpp"

namespace Rendering2D
{
	namespace SDL
	{
		typedef struct Texture
		{
			SDL_Texture* data;
			Rect pixels;
		}Texture;
	}
}