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
#include "foundations/resources/default_assets/fonts/lato_regular_ttf.hpp"
#include <unordered_map>
#include <foundations/singletons/platform/platform.hpp>

Omnific::Font::Font() 
{ 
	this->type = TYPE_STRING;
	const uint16_t default_font_size = 16;
	SDL_IOStream* rw = SDL_IOFromConstMem(DefaultAssets::lato_regular_ttf, DefaultAssets::lato_regular_ttf_len);
	TTF_Font* font = TTF_OpenFontIO(rw, 1, default_font_size);
	this->font = std::shared_ptr<TTF_Font>(font, TTF_CloseFont);
};

Omnific::Font::Font(Omnific::Font::HorizontalAlignment horizontal_alignment)
{
	this->horizontal_alignment = horizontal_alignment;
	this->type = TYPE_STRING;
	const uint16_t default_font_size = 16;
	SDL_IOStream* rw = SDL_IOFromConstMem(DefaultAssets::lato_regular_ttf, DefaultAssets::lato_regular_ttf_len);
	TTF_Font* font = TTF_OpenFontIO(rw, 1, default_font_size);
	TTF_SetFontWrapAlignment(font, (TTF_HorizontalAlignment)horizontal_alignment_to_sdl_value[horizontal_alignment]);	
	this->font = std::shared_ptr<TTF_Font>(font, TTF_CloseFont);

}

Omnific::Font::Font(Omnific::Font::HorizontalAlignment horizontal_alignment, uint16_t font_size)
{
	this->horizontal_alignment = horizontal_alignment;
	this->font_size = font_size;
	this->type = TYPE_STRING;
	const uint16_t default_font_size = 16;
	SDL_IOStream* rw = SDL_IOFromConstMem(DefaultAssets::lato_regular_ttf, DefaultAssets::lato_regular_ttf_len);
	TTF_Font* font = TTF_OpenFontIO(rw, 1, font_size);
	TTF_SetFontWrapAlignment(font, (TTF_HorizontalAlignment)horizontal_alignment_to_sdl_value[horizontal_alignment]);
	this->font = std::shared_ptr<TTF_Font>(font, TTF_CloseFont);
}

Omnific::Font::Font(std::string filepath)
{
	this->font_size = 16;
	this->set_name(filepath);
	this->type = TYPE_STRING;
	if (Platform::get_file_access().is_file_type(filepath, "ttf"))
		this->font = std::shared_ptr<TTF_Font>(TTF_OpenFont(filepath.c_str(), this->font_size), TTF_CloseFont);
}

Omnific::Font::Font(std::string filepath, uint16_t size_px)
{
	this->font_size = size_px;
	this->set_name(filepath);
	this->type = TYPE_STRING;
	if (Platform::get_file_access().is_file_type(filepath, "ttf"))
		this->font = std::shared_ptr<TTF_Font>(TTF_OpenFont(filepath.c_str(), this->font_size), TTF_CloseFont);
}

Omnific::Font::Font(std::string filepath, uint16_t size_px, HorizontalAlignment horizontal_alignment)
{
	this->font_size = size_px;
	this->horizontal_alignment = horizontal_alignment;
	this->set_name(filepath);
	this->type = TYPE_STRING;
	if (Platform::get_file_access().is_file_type(filepath, "ttf"))
	{
		TTF_Font* font = TTF_OpenFont(filepath.c_str(), this->font_size);
		TTF_SetFontWrapAlignment(font, (TTF_HorizontalAlignment)horizontal_alignment_to_sdl_value[horizontal_alignment]);
		this->font = std::shared_ptr<TTF_Font>(font, TTF_CloseFont);
	}
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