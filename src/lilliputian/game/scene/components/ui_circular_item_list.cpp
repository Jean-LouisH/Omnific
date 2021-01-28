#include "ui_circular_item_list.hpp"

void Lilliputian::UICircularItemList::setLowerBound(uint32_t lowerBound)
{
	this->lowerBound = lowerBound;

	if (this->index < lowerBound)
		this->index = lowerBound;
}

void Lilliputian::UICircularItemList::setUpperBound(uint32_t upperBound)
{
	this->upperBound = upperBound;

	if (this->index > upperBound)
		this->index = upperBound;
}

void Lilliputian::UICircularItemList::increment()
{

}

void Lilliputian::UICircularItemList::decrement()
{

}

void Lilliputian::UICircularItemList::setIndex(uint32_t index)
{
	this->index = index;

	if (this->index < lowerBound)
		this->index = lowerBound;
	else if (this->index > upperBound)
		this->index = upperBound;
}

uint32_t Lilliputian::UICircularItemList::getIndex()
{
	return this->index;
}