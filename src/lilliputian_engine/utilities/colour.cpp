#include "colour.hpp"

Lilliputian::Colour::Colour()
{
	this->red = 1.0;
	this->green = 1.0;
	this->blue = 1.0;
	this->alpha = 1.0;
}

Lilliputian::Colour::Colour(std::string hex)
{

}

Lilliputian::Colour::Colour(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
	this->red = float(red) / 255.0;
	this->green = float(green) / 255.0;
	this->blue = float(blue) / 255.0;
	this->alpha = float(alpha) / 255.0;
}