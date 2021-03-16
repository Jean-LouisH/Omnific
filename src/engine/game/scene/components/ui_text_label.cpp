#include "ui_text_label.hpp"

void Lilliputian::UITextLabel::setText(String text)
{
	this->text = text;
	this->generateTexture();
}

void Lilliputian::UITextLabel::setFont(Font font)
{
	this->font = font;
}

void Lilliputian::UITextLabel::setDimensions(float width, float height)
{
	this->rectangle.width = width;
	this->rectangle.height = height;
}

void Lilliputian::UITextLabel::setColour(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
	this->colour = Colour(red, green, blue, alpha);
}

void Lilliputian::UITextLabel::generateTexture()
{
	this->texture.unload();
	this->texture = Texture(this->text, this->font, this->colour, this->rectangle);
}