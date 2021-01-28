#pragma once

#include <SDL_render.h>
#include <vector>
#include "sprite_2d.hpp"
#include "sdl_renderable.hpp"
#include "camera_2d.hpp"

namespace SDL
{
	namespace Rendering2D
	{
		void buildRenderablesFromSprites(
			std::vector<SDLRenderable>* SDLRenderables,
			std::vector<Sprite2D> sprite2Ds,
			Camera2D camera2D,
			SDL_Window* window);
	}
}
