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

#include "sprite.hpp"


void Esi::Sprite::setImage(std::shared_ptr<Image> image)
{
	this->image = image;
}

void Esi::Sprite::setAlpha(uint8_t value)
{
	this->image->setAlpha(value);
}

uint8_t Esi::Sprite::getAlpha()
{
	return this->image->getAlpha();
}

void Esi::Sprite::hide()
{
	this->image->setAlpha(0);
}

void Esi::Sprite::show()
{
	this->image->setAlpha(255);
}

void Esi::Sprite::flipVertically()
{

}

void Esi::Sprite::flipHorizontally()
{

}

Esi::Image& Esi::Sprite::getImage()
{
	return *this->image;
}

std::string Esi::Sprite::getType() const
{
	return TYPE_STRING;
}