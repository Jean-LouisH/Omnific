#pragma once

#include "utilities/aliases.hpp"
#include "utilities/rectangle.hpp"
#include <SDL_surface.h>
#include <SDL_render.h>
#include <stdint.h>
#include "utilities/image.hpp"
#include "asset.hpp"
#include "font.hpp"
#include "utilities/rectangle.hpp"
#include "utilities/string.hpp"
#include "utilities/colour.hpp"

namespace Lilliputian
{
	class Texture : public Asset
	{
	public:
		Texture();
		Texture(Image image);
		Texture(String text, Font font, Colour colour, Rectangle rectangle);
		SDL_Surface* getSDLSurface();
		uint32_t getWidth();
		uint32_t getHeight();
		Rectangle getDimensions();
		virtual void unload();
	private:
		Image image;
	};
}