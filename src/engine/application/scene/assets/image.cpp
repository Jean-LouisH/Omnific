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
#include <vector>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

Omnia::Image::Image(std::string text, std::shared_ptr<Font> font, Colour colour, Font::RenderMode mode)
{
	this->type = TYPE_STRING;

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

Omnia::Image::Image(std::shared_ptr<Colour> colour)
{
	this->type = TYPE_STRING;
	const uint16_t size = 256;
	this->height = size;
	this->width = size;
	this->colourChannels = 3;
	size_t dataSize = this->width * this->height * this->colourChannels;
	uint32_t fillColour = colour->get24BitValue();

	for (int y = 0; y < this->height; y++)
		for (int x = 0; x < this->width; x++)
			this->colourPixel(fillColour, x, y);

}

Omnia::Image::Image(std::string filepath)
{
	this->type = TYPE_STRING;
	this->setName(filepath);

	if (filepath == "Image::default")
		this->setToDefault();
	else
		this->data = std::shared_ptr<uint8_t>(stbi_load(filepath.c_str(), &this->width, &this->height, &this->colourChannels, 0), stbi_image_free);
}

void* Omnia::Image::getData()
{
	if (this->data != nullptr)
		return this->data.get();
	else
		return nullptr;
}

uint32_t Omnia::Image::getWidth()
{
	if (this->data != nullptr)
		return this->width;
	else
		return 0;
}

uint32_t Omnia::Image::getHeight()
{
	if (this->data != nullptr)
		return this->height;
	else
		return 0;
}

uint32_t Omnia::Image::getDepth()
{
	return this->getBytesPerPixel() * 8;
}

uint32_t Omnia::Image::getPitch()
{
	return this->getBytesPerPixel() * this->getWidth();
}

Omnia::Rectangle Omnia::Image::getDimensions()
{
	Rectangle dimensions;
	dimensions.width = this->getWidth();
	dimensions.height = this->getHeight();
	return dimensions;
}

uint8_t Omnia::Image::getBytesPerPixel()
{
	return this->colourChannels;
}

void Omnia::Image::colourPixel(uint32_t fillColour, int x, int y)
{
	for (int colourChannel = 0; colourChannel < this->colourChannels; colourChannel++)
	{
		const int colourChannelByteLength = 8;
		int imageIndex = (y * this->width * this->colourChannels) + (x * this->colourChannels) + colourChannel;
		int maskByteShift = (this->colourChannels * colourChannelByteLength) - colourChannelByteLength;
		int colourChannelByteOffset = colourChannel * colourChannelByteLength;
		this->data.get()[imageIndex] = (fillColour & (0xFF0000 >> colourChannelByteOffset)) >> (maskByteShift - colourChannelByteOffset);
	}
}

void Omnia::Image::setToDefault()
{
	const uint32_t lighterGrey = 0x333333;
	const uint32_t darkerGrey = 0x2d2d2d;
	const uint16_t size = 256;
	const uint8_t divisions = 4;
	this->height = size;
	this->width = size;
	this->colourChannels = 3;
	size_t dataSize = this->width * this->height * this->colourChannels;

	this->data = std::shared_ptr<uint8_t>(new uint8_t[dataSize]);
	uint32_t fillColour = 0;
	bool darker = false;

	for (int y = 0; y < this->height; y++)
	{
		for (int x = 0; x < this->width; x++)
		{
			if (darker)
				fillColour = darkerGrey;
			else
				fillColour = lighterGrey;

			this->colourPixel(fillColour, x, y);

			if ((x % (size / divisions)) == 0)
				darker = !darker;
		}

		if ((y % (size / divisions)) == 0)
			darker = !darker;
	}
}