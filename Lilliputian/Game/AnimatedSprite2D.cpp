#include "AnimatedSprite2D.hpp"

void Lilliputian::AnimatedSprite2D::addTextureFrame(Texture texture)
{
	this->textureFrames.push_back(texture);
}

void Lilliputian::AnimatedSprite2D::flipHorizontally()
{
	this->flip ^= FLIP_HORIZONTAL;
}

void Lilliputian::AnimatedSprite2D::flipVertically()
{
	this->flip ^= FLIP_VERTICAL;
}

bool Lilliputian::AnimatedSprite2D::getVerticalFlip()
{
	return false;
}

bool Lilliputian::AnimatedSprite2D::getHorizontalFlip()
{
	return false;
}

Lilliputian::Vector<Lilliputian::Texture> Lilliputian::AnimatedSprite2D::getTextureFrames()
{
	return this->textureFrames;
}