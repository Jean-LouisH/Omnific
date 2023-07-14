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

#include "core/asset.hpp"
#include "stdint.h"
#include <core/utilities/aliases.hpp>
#include <string>
#include <memory>
#include <SDL_ttf.h>

namespace Omnia
{
	class OMNIA_ENGINE_API Font : public Asset
	{
	public:
		static constexpr const char* TYPE_STRING = "Font";

		Font() 
		{ 
			this->type = TYPE_STRING;
		};
		Font(std::string filepath);
		Font(std::string filepath, uint16_t size_px);
		Font(TTF_Font* font);

		virtual Registerable* instance() override
		{
			Font* clone = new Font(*this);
			clone->id = UIDGenerator::getNewUID();
			return clone;
		}
		TTF_Font* getSDLTTFFont();
		uint16_t getFontSize();
	private:
		std::shared_ptr<TTF_Font> font = { nullptr, TTF_CloseFont };
		uint16_t fontSize = 0;
	};
}