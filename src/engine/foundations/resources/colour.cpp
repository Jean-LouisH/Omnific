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
#include <sstream>
#include <iostream>

Omnific::Colour::Colour(std::string hex)
{
	uint32_t value;
	std::stringstream ss;
	std::string input_string;

	if (hex[0] == '#')
		input_string = hex.substr(1, hex.length());
	else
		input_string = hex;

	ss << std::hex << input_string;
	ss >> value;

	if (input_string.length() == 6)
		this->set_rgb(value);
	else if (input_string.length() == 8)
		this->set_rgba(value);

	this->type = TYPE_STRING;
}

Omnific::Colour::Colour(uint32_t value)
{
	this->set_rgba(value);
	this->type = TYPE_STRING;
}

Omnific::Colour::Colour(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
	this->red = float(red) / 255.0;
	this->green = float(green) / 255.0;
	this->blue = float(blue) / 255.0;
	this->alpha = float(alpha) / 255.0;
	this->type = TYPE_STRING;
}

Omnific::Colour::Colour(double red, double green, double blue, double alpha)
{
	this->red = red;
	this->green = green;
	this->blue = blue;
	this->alpha = alpha;
	this->type = TYPE_STRING;
}

uint8_t Omnific::Colour::get_red()
{
	return (uint8_t)(this->red * 255.0);
}

uint8_t Omnific::Colour::get_green()
{
	return (uint8_t)(this->green * 255.0);
}


uint8_t Omnific::Colour::get_blue()
{
	return (uint8_t)(this->blue * 255.0);
}

uint8_t Omnific::Colour::get_alpha()
{
	return (uint8_t)(this->alpha * 255.0);
}

void Omnific::Colour::set_rgb(uint32_t value)
{
	this->red = ((value & 0xFF0000) >> 16) / 255.0;
	this->green = ((value & 0x00FF00) >> 8) / 255.0;
	this->blue = ((value & 0x0000FF) >> 0) / 255.0;
	this->alpha = 1.0;
}

void Omnific::Colour::set_rgba(uint32_t value)
{
	this->red = ((value & 0xFF000000) >> 24) / 255.0;
	this->green = ((value & 0x00FF0000) >> 16) / 255.0;
	this->blue = ((value & 0x0000FF00) >> 8) / 255.0;
	this->alpha = ((value & 0x000000FF) >> 0) / 255.0;
}

uint32_t Omnific::Colour::get_rgb()
{
	return (uint32_t)((this->get_red() << 16) + (this->get_green() << 8) + (this->get_blue() << 0));
}

uint32_t Omnific::Colour::get_rgba()
{
	return (uint32_t)((this->get_red() << 24) + (this->get_green() << 16) + (this->get_blue() << 8) + (this->get_alpha() << 0));
}

glm::vec4 Omnific::Colour::get_rgba_in_vec4()
{
	return glm::vec4(this->red, this->green, this->blue, this->alpha);
}