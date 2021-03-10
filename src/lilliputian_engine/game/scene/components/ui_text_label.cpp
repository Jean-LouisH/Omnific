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

void Lilliputian::UITextLabel::generateTexture()
{

}