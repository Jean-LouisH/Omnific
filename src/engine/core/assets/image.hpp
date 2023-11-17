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

#include "core/utilities/aliases.hpp"
#include <SDL_surface.h>
#include <SDL_render.h>
#include <stdint.h>
#include "core/asset.hpp"
#include "font.hpp"
#include <string>
#include "colour.hpp"
#include "font.hpp"
#include <string>
#include <memory>
#include <glm/glm.hpp>

namespace Omnia
{
	class OMNIA_ENGINE_API Image : public Asset
	{
	public:
		enum class Alignment
		{
			TOP_LEFT,
			TOP_RIGHT,
			CENTRE
		};

		static constexpr const char* TYPE_STRING = "Image";
		Image() 
		{ 
			this->type = TYPE_STRING;
		};
		Image(std::string text, std::shared_ptr<Font> font, std::shared_ptr<Colour> colour, uint16_t wrapLength);
		Image(std::shared_ptr<Colour> colour);
		Image(std::shared_ptr<Colour> colour, int width, int height);
		Image(uint8_t* data, int width, int height, int colourChannels);
		Image(std::string filepath);

		virtual Registerable* instance() override
		{
			Image* clone = new Image(*this);
			clone->id = UIDGenerator::getNewUID();
			return clone;
		}
		uint8_t* getData();
		uint32_t getWidth();
		uint32_t getHeight();
		uint32_t getDepth();
		uint32_t getPitch();
		glm::vec2 getDimensions();
		uint8_t getBytesPerPixel();
		Alignment getAlignment();

		static void normalBlend(
			uint8_t* lowerImageData,
			glm::vec2 lowerPosition,
			glm::vec2 lowerDimensions,
			uint8_t* upperImageData,
			glm::vec2 upperPosition,
			glm::vec2 upperDimensions
		);
	private:
		std::shared_ptr<uint8_t> data = {nullptr, stbi_image_free };
		int width = 0;
		int height = 0;
		int colourChannels = 0;
		Alignment alignment;

		void colourPixel(uint32_t fillColour, int x, int y);
		void setToDefault();
		void setToColour(std::shared_ptr<Colour> colour, int width, int height);
		void setToParameters(int colourChannels, int width, int height, uint8_t* data);
	};
}