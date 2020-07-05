#pragma once

#include <SDL_render.h>

namespace Rendering2D
{
	namespace SDL
	{
		typedef struct SDLRenderable
		{
			SDL_Texture* texture;
			SDL_Rect textureRect;
			SDL_Rect renderingRect;
			double rotation;
			SDL_RendererFlip flip;
		}SDLRenderable;
	}
}
