#include "Texture.hpp"
#include "../Utilities/Rectangle.hpp"
#include <SDL_image.h>

Lilliputian::Texture::Texture(const char* filepath)
{
	this->surface = IMG_Load(filepath);
}

Lilliputian::Texture::~Texture()
{
	if (this->surface != NULL)
		SDL_FreeSurface(this->surface);
}

SDL_Surface* Lilliputian::Texture::getSDLSurface()
{
	return this->surface;
}

double Lilliputian::Texture::getWidth()
{
	return this->surface->w;
}

double Lilliputian::Texture::getHeight()
{
	return this->surface->h;
}

Lilliputian::Rectangle Lilliputian::Texture::getDimensions()
{
	Rectangle dimensions;
	dimensions.width = this->getWidth();
	dimensions.height = this->getHeight();
	return dimensions;
}