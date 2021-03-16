#include "texture.hpp"

Lilliputian::Texture::Texture()
{

}

Lilliputian::Texture::Texture(Image image)
{
	this->image = image;
}

Lilliputian::Texture::Texture(String text, Font font, Colour colour, Rectangle rectangle)
{

}

void Lilliputian::Texture::unload()
{
	this->image.unload();
}

SDL_Surface* Lilliputian::Texture::getSDLSurface()
{
	return this->image.getSDLSurface();
}

uint32_t Lilliputian::Texture::getWidth()
{
	if (this->image.getSDLSurface() != NULL)
		return this->image.getWidth();
	else
		return 0;
}

uint32_t Lilliputian::Texture::getHeight()
{
	if (this->image.getSDLSurface() != NULL)
		return this->image.getHeight();
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