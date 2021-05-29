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

#include "image.hpp"
#include <game/scene/id_counter.hpp>
#include <SDL_image.h>

Lilliputian::Image::Image()
{
	this->surface = nullptr;
}

Lilliputian::Image::Image(String text, Font font, Colour colour, Font::RenderMode mode)
{
	SDL_Color sdlColor = { colour.getRed(), colour.getGreen(), colour.getBlue(), colour.getAlpha() };
	SDL_Color sdlBackgroundColor = { 0, 0, 0, 255 };

	switch (mode)
	{
	case Font::RenderMode::SOLID:
		this->surface = TTF_RenderUTF8_Solid(font.getSDLTTFFont(), text.c_str(), sdlColor);
		break;
	case Font::RenderMode::SHADED:
		this->surface = TTF_RenderUTF8_Shaded(font.getSDLTTFFont(), text.c_str(), sdlColor, sdlBackgroundColor);
		break;
	case Font::RenderMode::BLENDED:
		this->surface = TTF_RenderUTF8_Blended(font.getSDLTTFFont(), text.c_str(), sdlColor);
		break;
	}

	this->id = IDCounter::getNewID();
}

Lilliputian::Image::Image(const char* filepath)
{
	this->surface = IMG_Load(filepath);
	this->id = IDCounter::getNewID();
}

Lilliputian::Image::Image(SDL_Surface* surface)
{
	this->surface = surface;
	this->id = IDCounter::getNewID();
}

void Lilliputian::Image::unload()
{
	if (this->surface != nullptr)
	{
		SDL_FreeSurface(this->surface);
		this->surface = nullptr;
	}
}

SDL_Surface* Lilliputian::Image::getSDLSurface()
{
	if (this->surface != nullptr)
		return this->surface;
	else
		return nullptr;
}

uint32_t Lilliputian::Image::getWidth()
{
	if (this->surface != nullptr)
		return this->surface->w;
	else
		return 0;
}

uint32_t Lilliputian::Image::getHeight()
{
	if (this->surface != nullptr)
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

void Lilliputian::Image::setAlpha(uint8_t value)
{
	this->alpha = value;
}

uint8_t Lilliputian::Image::getAlpha()
{
	return this->alpha;
}

Lilliputian::AssetID Lilliputian::Image::getID()
{
	return this->id;
}