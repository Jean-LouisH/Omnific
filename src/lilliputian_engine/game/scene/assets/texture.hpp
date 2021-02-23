#pragma once

#include "utilities/aliases.hpp"
#include "utilities/rectangle.hpp"
#include <SDL_surface.h>
#include <SDL_render.h>
#include <stdint.h>
#include "utilities/image.hpp"
#include "asset.hpp"

namespace Lilliputian
{
	class Texture : public Asset
	{
	public:
		Texture(Image* image);
		SDL_Surface* getSDLSurface();
		uint32_t getWidth();
		uint32_t getHeight();
		Rectangle getDimensions();
		virtual void unload();
	private:
		Image* image = NULL;
	};
}