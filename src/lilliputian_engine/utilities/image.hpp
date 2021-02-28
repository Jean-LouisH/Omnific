#pragma once

#include "aliases.hpp"
#include "rectangle.hpp"
#include <SDL_surface.h>
#include <SDL_render.h>
#include <stdint.h>

namespace Lilliputian
{
	using ImageHash = uint64_t;

	class Image
	{
	public:
		Image();
		Image(const char* filepath);
		void unload();
		ImageHash hash();
		SDL_Surface* getSDLSurface();
		uint32_t getWidth();
		uint32_t getHeight();
		Rectangle getDimensions();
	private:
		SDL_Surface* surface;
	};
}