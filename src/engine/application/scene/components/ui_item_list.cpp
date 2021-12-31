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

#include "ui_item_list.hpp"

void Esi::UIItemList::setLowerBound(uint32_t lowerBound)
{
	this->lowerBound = lowerBound;

	if (this->index < lowerBound)
		this->index = lowerBound;
}

void Esi::UIItemList::setUpperBound(uint32_t upperBound)
{
	this->upperBound = upperBound;

	if (this->index > upperBound)
		this->index = upperBound;
}

void Esi::UIItemList::increment()
{
	this->index++;

	if (this->index > upperBound)
		this->index = lowerBound;
}

void Esi::UIItemList::decrement()
{
	this->index--;

	if (this->index < lowerBound)
		this->index = upperBound;
}

void Esi::UIItemList::setIndex(uint32_t index)
{
	this->index = index;

	if (this->index < lowerBound)
		this->index = lowerBound;
	else if (this->index > upperBound)
		this->index = upperBound;
}

uint32_t Esi::UIItemList::getIndex()
{
	return this->index;
}

Esi::Image& Esi::UIItemList::getImage()
{
	return this->image;
}

std::string Esi::UIItemList::getType() const
{
	return "UIItemList";
}
