#pragma once

#include "../../../Utilities/Aliases.hpp"
#include "../../../Utilities/Rectangle.hpp"
#include <SDL_surface.h>
#include <SDL_render.h>
#include <stdint.h>
#include "../../../Utilities/Image.hpp"

namespace Lilliputian
{
	class Texture
	{
	private:
		Image* image = NULL;
	public:
		Texture(Image* image);
		void unload();
		SDL_Surface* getSDLSurface();
		uint32_t getWidth();
		uint32_t getHeight();
		Rectangle getDimensions();
	};
}