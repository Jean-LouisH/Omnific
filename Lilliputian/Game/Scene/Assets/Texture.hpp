#pragma once

#include "../../../Utilities/Aliases.hpp"
#include "../../../Utilities/Rectangle.hpp"
#include <SDL_surface.h>
#include <SDL_render.h>
#include <stdint.h>
#include "../../../Utilities/Image.hpp"
#include "Asset.hpp"

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