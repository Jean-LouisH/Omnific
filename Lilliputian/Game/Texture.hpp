#pragma once

#include "../Utilities/Aliases.hpp"
#include "../Utilities/Rectangle.hpp"
#include <SDL_surface.h>
#include <SDL_render.h>

namespace Lilliputian
{
	class Texture
	{
	private:
		SDL_Surface* surface;
	public:
		Texture(const char* filepath);
		~Texture();
		SDL_Surface* getSDLSurface();
		double getWidth();
		double getHeight();
		Rectangle getDimensions();
	};
}