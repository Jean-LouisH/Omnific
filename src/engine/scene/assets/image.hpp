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

#include "stb_image.h"
#include "stb_image_write.h"

#include "utilities/aliases.hpp"
#include "utilities/rectangle.hpp"
#include <SDL_surface.h>
#include <SDL_render.h>
#include <stdint.h>
#include "scene/assets/asset.hpp"
#include "font.hpp"
#include "utilities/rectangle.hpp"
#include <string>
#include "utilities/colour.hpp"
#include "font.hpp"
#include <string>
#include <memory>

namespace Omnia
{
	class OMNIA_ENGINE_API Image : public Asset
	{
	public:
		static constexpr const char* TYPE_STRING = "Image";
		Image() 
		{ 
			this->type = TYPE_STRING;
		};
		Image(std::string text, std::shared_ptr<Font> font, Colour colour, Font::RenderMode mode);
		Image(std::shared_ptr<Colour> colour);
		Image(uint8_t* data, int width, int height, int colourChannels);
		Image(std::string filepath);

		virtual Registerable* clone() override
		{
			Image* clone = new Image(*this);
			clone->id = UIDGenerator::getNewUID();
			return clone;
		}
		void* getData();
		uint32_t getWidth();
		uint32_t getHeight();
		uint32_t getDepth();
		uint32_t getPitch();
		Rectangle getDimensions();
		uint8_t getBytesPerPixel();
	private:
		std::shared_ptr<uint8_t> data = {nullptr, stbi_image_free };
		int width = 0;
		int height = 0;
		int colourChannels = 0;

		void colourPixel(uint32_t fillColour, int x, int y);
		void setToDefault();
		void setToParameters(int colourChannels, int width, int height, uint8_t* data);
	};
}