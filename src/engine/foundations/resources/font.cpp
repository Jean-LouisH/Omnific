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

#include "font.hpp"

Omnific::Font::Font(std::string filepath)
{
	this->font_size = 16;
	this->set_name(filepath);
	this->type = TYPE_STRING;
	this->font = std::shared_ptr<TTF_Font>(TTF_OpenFont(filepath.c_str(), this->font_size), TTF_CloseFont);
}

Omnific::Font::Font(std::string filepath, uint16_t size_px)
{
	this->font_size = size_px;
	this->set_name(filepath);
	this->type = TYPE_STRING;
	this->font = std::shared_ptr<TTF_Font>(TTF_OpenFont(filepath.c_str(), this->font_size), TTF_CloseFont);
}

Omnific::Font::Font(TTF_Font* font)
{
	this->type = TYPE_STRING;
	this->font = std::shared_ptr<TTF_Font>(font, TTF_CloseFont);
}

TTF_Font* Omnific::Font::get_sdl_ttf_font()
{
	return this->font.get();
}

uint16_t Omnific::Font::get_font_size()
{
	return this->font_size;
}