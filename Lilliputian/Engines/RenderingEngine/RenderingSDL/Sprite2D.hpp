#pragma once

#include <SDL_render.h>
#include <stdint.h>
#include <vector>
#include "Texture.hpp"
#include "Transform2D.hpp"

namespace Rendering2D
{
	namespace SDL
	{
		typedef struct Sprite2D
		{
			Transform2D transform;
			std::vector<Texture> textureFrames;
			uint8_t frameIndex;
			uint8_t alpha;
			SDL_RendererFlip flip;

			Sprite2D() :
				frameIndex(0),
				alpha(255),
				flip(SDL_FLIP_NONE)
			{}
		}Sprite2D;
	}
}