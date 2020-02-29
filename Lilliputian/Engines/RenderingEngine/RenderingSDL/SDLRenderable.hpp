#pragma once

#include <SDL_render.h>

typedef struct SDLRenderable
{
	SDL_Texture* texture;
	SDL_Rect textureRect;
	SDL_Rect renderingRect;
	double rotation;
	SDL_RendererFlip flip;
}SDLRenderable;
