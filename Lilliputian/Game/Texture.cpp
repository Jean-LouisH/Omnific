#include "Texture.hpp"
#include "../Utilities/Rectangle.hpp"
#include <SDL_image.h>

Lilliputian::Texture::Texture(const char* filepath)
{
	this->surface = IMG_Load(filepath);
}

void Lilliputian::Texture::unload()
{
	if (this->surface != NULL)
		SDL_FreeSurface(this->surface);
}

SDL_Surface* Lilliputian::Texture::getSDLSurface()
{
	if (this->surface != NULL)
		return this->surface;
	else
		return NULL;
}

double Lilliputian::Texture::getWidth()
{
	if (this->surface != NULL)
		return this->surface->w;
	else
		return 0.0;
}

double Lilliputian::Texture::getHeight()
{
	if (this->surface != NULL)
		return this->surface->h;
	else
		return 0.0;
}

Lilliputian::Rectangle Lilliputian::Texture::getDimensions()
{
	Rectangle dimensions;
	dimensions.width = this->getWidth();
	dimensions.height = this->getHeight();
	return dimensions;
}