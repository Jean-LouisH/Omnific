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

#include "window_api.hpp"

void Lilliputian::WindowAPI::initialize(Window* window)
{
	this->window = window;
}

void Lilliputian::WindowAPI::setToWindowed(uint16_t width_px, uint16_t height_px)
{
	this->window->setToWindowed(width_px, height_px);
}

void Lilliputian::WindowAPI::setToFullscreen()
{
	this->window->setToFullscreen();
}

void Lilliputian::WindowAPI::toggleWindowedFullscreen()
{
	this->window->toggleWindowedFullscreen();
}

void Lilliputian::WindowAPI::resize(uint16_t width_px, uint16_t height_px)
{
	this->window->resize(width_px, height_px);
}

void Lilliputian::WindowAPI::changeTitle(std::string title)
{
	this->window->changeTitle(title.c_str());
}

void Lilliputian::WindowAPI::changeIcon(Image image)
{
	this->window->changeIcon(image);
}

void Lilliputian::WindowAPI::maximize()
{
	this->window->maximize();
}

void Lilliputian::WindowAPI::minimize()
{
	this->window->minimize();
}

void Lilliputian::WindowAPI::raise()
{
	this->window->raise();
}

void Lilliputian::WindowAPI::restore()
{
	this->window->restore();
}

void Lilliputian::WindowAPI::hide()
{
	this->window->hide();
}

void Lilliputian::WindowAPI::show()
{
	this->window->show();
}