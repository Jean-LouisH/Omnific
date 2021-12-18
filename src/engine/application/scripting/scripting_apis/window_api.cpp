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

Esi::WindowAPI::WindowAPI(Window* window)
{
	this->window = window;
}

void Esi::WindowAPI::setToWindowed(uint16_t width_px, uint16_t height_px)
{
	this->window->setToWindowed(width_px, height_px);
}

void Esi::WindowAPI::setToFullscreen()
{
	this->window->setToFullscreen();
}

void Esi::WindowAPI::toggleWindowedFullscreen()
{
	this->window->toggleWindowedFullscreen();
}

void Esi::WindowAPI::resize(uint16_t width_px, uint16_t height_px)
{
	this->window->resize(width_px, height_px);
}

void Esi::WindowAPI::changeTitle(std::string title)
{
	this->window->changeTitle(title.c_str());
}

void Esi::WindowAPI::changeIcon(Image image)
{
	this->window->changeIcon(image);
}

void Esi::WindowAPI::maximize()
{
	this->window->maximize();
}

void Esi::WindowAPI::minimize()
{
	this->window->minimize();
}

void Esi::WindowAPI::raise()
{
	this->window->raise();
}

void Esi::WindowAPI::restore()
{
	this->window->restore();
}

void Esi::WindowAPI::hide()
{
	this->window->hide();
}

void Esi::WindowAPI::show()
{
	this->window->show();
}