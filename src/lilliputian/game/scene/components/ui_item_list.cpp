#include "ui_item_list.hpp"

void Lilliputian::UIItemList::setLowerBound(uint32_t lowerBound)
{
	this->lowerBound = lowerBound;

	if (this->index < lowerBound)
		this->index = lowerBound;
}

void Lilliputian::UIItemList::setUpperBound(uint32_t upperBound)
{
	this->upperBound = upperBound;

	if (this->index > upperBound)
		this->index = upperBound;
}

void Lilliputian::UIItemList::increment()
{
	this->index++;

	if (this->index > upperBound)
		this->index = lowerBound;
}

void Lilliputian::UIItemList::decrement()
{
	this->index--;

	if (this->index < lowerBound)
		this->index = upperBound;
}

void Lilliputian::UIItemList::setIndex(uint32_t index)
{
	this->index = index;

	if (this->index < lowerBound)
		this->index = lowerBound;
	else if (this->index > upperBound)
		this->index = upperBound;
}

uint32_t Lilliputian::UIItemList::getIndex()
{
	return this->index;
}