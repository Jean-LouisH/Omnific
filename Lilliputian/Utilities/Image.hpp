#pragma once

#include "Aliases.hpp"
#include "Rectangle.hpp"
#include <SDL_surface.h>
#include <SDL_render.h>
#include <stdint.h>

namespace Lilliputian
{
	class Image
	{
	public:
		Image(const char* filepath);
		void unload();
		SDL_Surface* getSDLSurface();
		uint32_t getWidth();
		uint32_t getHeight();
		Rectangle getDimensions();
	private:
		SDL_Surface* surface;
	};
}