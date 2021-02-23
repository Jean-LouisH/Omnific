#pragma once

#include <SDL_render.h>

namespace SDL
{
	namespace Rendering2D
	{
		typedef struct SDLRenderable
		{
			SDL_Texture* texture;
			SDL_Rect textureRect;
			SDL_Rect renderingRect;
			float rotation;
			SDL_RendererFlip flip;
		}SDLRenderable;
	}
}
