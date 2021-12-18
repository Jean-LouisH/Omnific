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

#include "colour.hpp"

Esi::Colour::Colour()
{
	this->red = 1.0;
	this->green = 1.0;
	this->blue = 1.0;
	this->alpha = 1.0;
}

Esi::Colour::Colour(std::string hex)
{

}

Esi::Colour::Colour(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
	this->red = float(red) / 255.0;
	this->green = float(green) / 255.0;
	this->blue = float(blue) / 255.0;
	this->alpha = float(alpha) / 255.0;
}

uint8_t Esi::Colour::getRed()
{
	return (uint8_t)(this->red * 255.0);
}

uint8_t Esi::Colour::getGreen()
{
	return (uint8_t)(this->green * 255.0);
}


uint8_t Esi::Colour::getBlue()
{
	return (uint8_t)(this->blue * 255.0);
}

uint8_t Esi::Colour::getAlpha()
{
	return (uint8_t)(this->alpha * 255.0);
}