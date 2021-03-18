#pragma once

#include "utilities/aliases.hpp"
#include "utilities/rectangle.hpp"
#include <SDL_surface.h>
#include <SDL_render.h>
#include <stdint.h>
#include "asset.hpp"
#include "font.hpp"
#include "utilities/rectangle.hpp"
#include "utilities/string.hpp"
#include "utilities/colour.hpp"

namespace Lilliputian
{
	class Image : public Asset
	{
	public:
		using ImageHash = uint64_t;

		Image();
		Image(String text, Font font, Colour colour, Rectangle rectangle);
		Image(const char* filepath);
		Image(SDL_Surface* surface);
		virtual void unload();
		SDL_Surface* getSDLSurface();
		uint32_t getWidth();
		uint32_t getHeight();
		Rectangle getDimensions();
	private:
		SDL_Surface* surface;
	};
}