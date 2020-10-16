#include "CircularItemList.hpp"

void Lilliputian::CircularItemList::setLowerBound(uint32_t lowerBound)
{
	this->lowerBound = lowerBound;

	if (this->index < lowerBound)
		this->index = lowerBound;
}

void Lilliputian::CircularItemList::setUpperBound(uint32_t upperBound)
{
	this->upperBound = upperBound;

	if (this->index > upperBound)
		this->index = upperBound;
}

void Lilliputian::CircularItemList::increment()
{

}

void Lilliputian::CircularItemList::decrement()
{

}

void Lilliputian::CircularItemList::setIndex(uint32_t index)
{
	this->index = index;

	if (this->index < lowerBound)
		this->index = lowerBound;
	else if (this->index > upperBound)
		this->index = upperBound;
}

uint32_t Lilliputian::CircularItemList::getIndex()
{
	return this->index;
}