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

#pragma once

#include "foundations/resources/resource.hpp"
#include "stdint.h"
#include <foundations/aliases.hpp>
#include <string>
#include <memory>
#include <SDL3_ttf/SDL_ttf.h>
#include <unordered_map>
#include <engine_api.hpp>


namespace Omnific
{
	class OMNIFIC_ENGINE_API Font : public Resource
	{
	public:
		static constexpr const char* TYPE_STRING = "Font";

		enum class HorizontalAlignment
		{
			LEFT,
			CENTER,
			RIGHT
		};

		Font();
		Font(HorizontalAlignment horizontal_alignment);
		Font(HorizontalAlignment horizontal_alignment, uint16_t font_size);
		Font(std::string filepath);
		Font(std::string filepath, uint16_t size_px);
		Font(std::string filepath, uint16_t size_px, HorizontalAlignment horizontal_alignment);
		Font(TTF_Font* font);

		virtual Registerable* instance() override
		{
			Font* clone = new Font(*this);
			clone->id = UIDGenerator::get_new_uid();
			return clone;
		}
		TTF_Font* get_sdl_ttf_font();
		uint16_t get_font_size();
	private:
		std::shared_ptr<TTF_Font> font = { nullptr, TTF_CloseFont };
		uint16_t font_size = 0;
		HorizontalAlignment horizontal_alignment = HorizontalAlignment::LEFT;
		std::unordered_map<HorizontalAlignment, int> horizontal_alignment_to_sdl_value = {
			{ HorizontalAlignment::LEFT, TTF_HORIZONTAL_ALIGN_LEFT },
			{ HorizontalAlignment::CENTER, TTF_HORIZONTAL_ALIGN_CENTER },
			{ HorizontalAlignment::RIGHT, TTF_HORIZONTAL_ALIGN_RIGHT }
		};
	};
}