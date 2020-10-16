#include "ItemList.hpp"

void Lilliputian::ItemList::setLowerBound(uint32_t lowerBound)
{
	this->lowerBound = lowerBound;

	if (this->index < lowerBound)
		this->index = lowerBound;
}

void Lilliputian::ItemList::setUpperBound(uint32_t upperBound)
{
	this->upperBound = upperBound;

	if (this->index > upperBound)
		this->index = upperBound;
}

void Lilliputian::ItemList::increment()
{
	this->index++;

	if (this->index > upperBound)
		this->index = lowerBound;
}

void Lilliputian::ItemList::decrement()
{
	this->index--;

	if (this->index < lowerBound)
		this->index = upperBound;
}

void Lilliputian::ItemList::setIndex(uint32_t index)
{
	this->index = index;

	if (this->index < lowerBound)
		this->index = lowerBound;
	else if (this->index > upperBound)
		this->index = upperBound;
}

uint32_t Lilliputian::ItemList::getIndex()
{
	return this->index;
}