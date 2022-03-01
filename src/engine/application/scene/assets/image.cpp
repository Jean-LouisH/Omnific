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

#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

Omnific::Image::Image(std::string text, std::shared_ptr<Font> font, Colour colour, Font::RenderMode mode)
{
	Image();
	//SDL_Color sdlColor = { colour.getRed(), colour.getGreen(), colour.getBlue(), colour.getAlpha() };
	//SDL_Color sdlBackgroundColor = { 0, 0, 0, 255 };

	//switch (mode)
	//{
	//case Font::RenderMode::SOLID:
	//	this->surface = std::shared_ptr<SDL_Surface>(TTF_RenderUTF8_Solid(font->getSDLTTFFont(), text.c_str(), sdlColor), SDL_FreeSurface);
	//	break;
	//case Font::RenderMode::SHADED:
	//	this->surface = std::shared_ptr<SDL_Surface>(TTF_RenderUTF8_Shaded(font->getSDLTTFFont(), text.c_str(), sdlColor, sdlBackgroundColor), SDL_FreeSurface);
	//	break;
	//case Font::RenderMode::BLENDED:
	//	this->surface = std::shared_ptr<SDL_Surface>(TTF_RenderUTF8_Blended(font->getSDLTTFFont(), text.c_str(), sdlColor), SDL_FreeSurface);
	//	break;
	//}
}

Omnific::Image::Image(std::string filepath)
{
	Image();
	this->setName(filepath);
	this->data = std::shared_ptr<uint8_t>(stbi_load(filepath.c_str(), &this->width, &this->height, &this->channels, 0), stbi_image_free);
}

void* Omnific::Image::getData()
{
	if (this->data != nullptr)
		return this->data.get();
	else
		return nullptr;
}

uint32_t Omnific::Image::getWidth()
{
	if (this->data != nullptr)
		return this->width;
	else
		return 0;
}

uint32_t Omnific::Image::getHeight()
{
	if (this->data != nullptr)
		return this->height;
	else
		return 0;
}

uint32_t Omnific::Image::getDepth()
{
	return this->getBytesPerPixel() * 8;
}

uint32_t Omnific::Image::getPitch()
{
	return this->getBytesPerPixel() * this->getWidth();
}

Omnific::Rectangle Omnific::Image::getDimensions()
{
	Rectangle dimensions;
	dimensions.width = this->getWidth();
	dimensions.height = this->getHeight();
	return dimensions;
}

void Omnific::Image::setAlpha(uint8_t value)
{
	this->alpha = value;
}

uint8_t Omnific::Image::getAlpha()
{
	return this->alpha;
}

uint8_t Omnific::Image::getBytesPerPixel()
{
	return this->channels;
}