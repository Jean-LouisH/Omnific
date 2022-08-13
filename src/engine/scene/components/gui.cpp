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

#include "gui.hpp"

void Omnia::GUIText::setText(std::string text)
{
	if (text != this->text)
	{
		this->text = text;
		this->generateImage();
	}
}

void Omnia::GUIText::setFont(std::shared_ptr<Omnia::Font> font, uint16_t size_px)
{
	this->font = font;
	this->size = size_px;
	this->generateImage();
}

void Omnia::GUIText::setFontStyleNormal()
{
	this->style = Font::Style::NORMAL;
	this->generateImage();
}

void Omnia::GUIText::setFontStyleBold()
{
	this->style = Font::Style::BOLD;
	this->generateImage();
}

void Omnia::GUIText::setFontStyleItalic()
{
	this->style = Font::Style::ITALIC;
	this->generateImage();
}

void Omnia::GUIText::setFontStyleUnderline()
{
	this->style = Font::Style::UNDERLINE;
	this->generateImage();
}

void Omnia::GUIText::setFontStyleStrikethrough()
{
	this->style = Font::Style::STRIKETHROUGH;
	this->generateImage();
}

void Omnia::GUIText::setFontRenderModeSolid()
{
	this->mode = Font::RenderMode::SOLID;
	this->generateImage();
}

void Omnia::GUIText::setFontRenderModeShaded()
{
	this->mode = Font::RenderMode::SHADED;
	this->generateImage();
}

void Omnia::GUIText::setFontRenderModeBlended()
{
	this->mode = Font::RenderMode::BLENDED;
	this->generateImage();
}

void Omnia::GUIText::setFontSize(uint16_t size_px)
{

	this->generateImage();
}

void Omnia::GUIText::setColour(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
	this->colour = Colour(red, green, blue, alpha);
	this->generateImage();
}

Omnia::Font::Style Omnia::GUIText::getFontStyle()
{
	return this->style;
}

void Omnia::GUIText::generateImage()
{
	if (this->font != nullptr)
	{
		if (this->font->getSDLTTFFont() != nullptr)
		{
			this->image = std::shared_ptr<Image>(new Image(this->text, this->font, this->colour, this->mode));
		}
	}
}