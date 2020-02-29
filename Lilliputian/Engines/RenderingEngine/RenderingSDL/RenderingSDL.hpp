#pragma once

#include <SDL_render.h>
#include <vector>
#include "Sprite2D.hpp"
#include "SDLRenderable.hpp"
#include "Camera2D.hpp"

namespace Rendering2D
{
	namespace SDL
	{
		void buildRenderablesFromSprites(
			std::vector<SDLRenderable>* SDLRenderables,
			std::vector<Sprite2D> sprite2Ds,
			Camera2D camera2D,
			SDL_Window* window);
	}
}
