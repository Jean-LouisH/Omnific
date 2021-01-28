#include "texture.hpp"

Lilliputian::Texture::Texture(Image* image)
{
	this->image = image;
}

void Lilliputian::Texture::unload()
{
	this->image = NULL;
}

SDL_Surface* Lilliputian::Texture::getSDLSurface()
{
	if (this->image != NULL)
		return this->image->getSDLSurface();
	else
		return NULL;
}

uint32_t Lilliputian::Texture::getWidth()
{
	if (this->image != NULL)
		return this->image->getWidth();
	else
		return 0;
}

uint32_t Lilliputian::Texture::getHeight()
{
	if (this->image != NULL)
		return this->image->getHeight();
	else
		return 0;
}

Lilliputian::Rectangle Lilliputian::Texture::getDimensions()
{
	Rectangle dimensions;
	dimensions.width = this->getWidth();
	dimensions.height = this->getHeight();
	return dimensions;
}