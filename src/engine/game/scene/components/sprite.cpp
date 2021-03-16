#include "sprite.hpp"


void Lilliputian::Sprite::setTexture(Texture texture)
{
	this->texture = texture;
}

void Lilliputian::Sprite::setAlpha(uint8_t value)
{

}

uint8_t Lilliputian::Sprite::getAlpha()
{
	return this->alpha;
}

void Lilliputian::Sprite::hide()
{
	this->alpha = 0;
}

void Lilliputian::Sprite::show()
{
	this->alpha = 255;
}

void Lilliputian::Sprite::flipVertically()
{

}

void Lilliputian::Sprite::flipHorizontally()
{

}

Lilliputian::Texture& Lilliputian::Sprite::getTexture()
{
	return this->texture;
}