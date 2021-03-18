#include "image.hpp"
#include <SDL_image.h>

Lilliputian::Image::Image()
{
	this->surface = NULL;
}

Lilliputian::Image::Image(String text, Font font, Colour colour, Rectangle rectangle)
{

}


Lilliputian::Image::Image(const char* filepath)
{
	this->surface = IMG_Load(filepath);
}

Lilliputian::Image::Image(SDL_Surface* surface)
{
	this->surface = surface;
}

void Lilliputian::Image::unload()
{
	if (this->surface != NULL)
		SDL_FreeSurface(this->surface);
}

SDL_Surface* Lilliputian::Image::getSDLSurface()
{
	if (this->surface != NULL)
		return this->surface;
	else
		return NULL;
}

uint32_t Lilliputian::Image::getWidth()
{
	if (this->surface != NULL)
		return this->surface->w;
	else
		return 0;
}

uint32_t Lilliputian::Image::getHeight()
{
	if (this->surface != NULL)
		return this->surface->h;
	else
		return 0;
}

Lilliputian::Rectangle Lilliputian::Image::getDimensions()
{
	Rectangle dimensions;
	dimensions.width = this->getWidth();
	dimensions.height = this->getHeight();
	return dimensions;
}