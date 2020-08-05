#include "Sprite2D.hpp"

void Lilliputian::Sprite2D::addTextureFrame(Texture texture)
{
	this->textureFrames.push_back(texture);
}

void Lilliputian::Sprite2D::flipHorizontally()
{
	this->flip ^= FLIP_HORIZONTAL;
}

void Lilliputian::Sprite2D::flipVertically()
{
	this->flip ^= FLIP_VERTICAL;
}

bool Lilliputian::Sprite2D::getVerticalFlip()
{
	return false;
}

bool Lilliputian::Sprite2D::getHorizontalFlip()
{
	return false;
}

Lilliputian::Vector<Lilliputian::Texture> Lilliputian::Sprite2D::getTextureFrames()
{
	return this->textureFrames;
}