// MIT License
// 
// Copyright (c) 2020 Jean-Louis Haywood
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "ui_text_label.hpp"


Esi::UITextLabel::UITextLabel()
{
	this->image = Image();
}

void Esi::UITextLabel::setText(std::string text)
{
	if (text != this->text)
	{
		this->text = text;
		this->generateImage();
	}
}

void Esi::UITextLabel::setFont(std::shared_ptr<Esi::Font> font, uint16_t size_px)
{
	this->font = font;
	this->size_px = size_px;
	this->generateImage();
}

void Esi::UITextLabel::setFontStyleNormal()
{
	this->style = Font::Style::NORMAL;
	this->generateImage();
}

void Esi::UITextLabel::setFontStyleBold()
{
	this->style = Font::Style::BOLD;
	this->generateImage();
}

void Esi::UITextLabel::setFontStyleItalic()
{
	this->style = Font::Style::ITALIC;
	this->generateImage();
}

void Esi::UITextLabel::setFontStyleUnderline()
{
	this->style = Font::Style::UNDERLINE;
	this->generateImage();
}

void Esi::UITextLabel::setFontStyleStrikethrough()
{
	this->style = Font::Style::STRIKETHROUGH;
	this->generateImage();
}

void Esi::UITextLabel::setFontRenderModeSolid()
{
	this->mode = Font::RenderMode::SOLID;
	this->generateImage();
}

void Esi::UITextLabel::setFontRenderModeShaded()
{
	this->mode = Font::RenderMode::SHADED;
	this->generateImage();
}

void Esi::UITextLabel::setFontRenderModeBlended()
{
	this->mode = Font::RenderMode::BLENDED;
	this->generateImage();
}

void Esi::UITextLabel::setFontSize(uint16_t size_px)
{

	this->generateImage();
}

void Esi::UITextLabel::setColour(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
	this->colour = Colour(red, green, blue, alpha);
	this->generateImage();
}

Esi::Font::Style Esi::UITextLabel::getFontStyle()
{
	return this->style;
}

Esi::Image& Esi::UITextLabel::getImage()
{
	return this->image;
}

void Esi::UITextLabel::setAlpha(uint8_t value)
{
	this->image.setAlpha(value);
}

uint8_t Esi::UITextLabel::getAlpha()
{
	return this->image.getAlpha();
}

void Esi::UITextLabel::generateImage()
{
	if (this->font != nullptr)
	{
		if (this->font->getSDLTTFFont() != nullptr)
		{
			this->image.unload();
			this->image = Image(this->text, this->font, this->colour, this->mode);
		}
	}
}

std::string Esi::UITextLabel::getType() const
{
	return TYPE_STRING;
}