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

#include "foundations/aliases.hpp"
#include <vector>
#include <unordered_map>
#include "foundations/constants.hpp"
#include <string>
#include "foundations/resources/image.hpp"
#include <scene/components/component.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <foundations/colour.hpp>
#include <foundations/resources/shader.hpp>

namespace Omnific
{
	class OMNIFIC_ENGINE_API WorldEnvironment : public Component
	{
	public:
		friend class RenderingSystem;
		enum class BackgroundMode
		{
			SKY,
			CLEAR_COLOUR
		};

		enum class ToneMapper
		{
			LINEAR,
			AGX,
			ACES
		};

		WorldEnvironment()
		{
			this->type = TYPE_STRING;
			this->clear_colour = std::shared_ptr<Colour>(new Colour("#888888"));
		};
		static constexpr const char* TYPE_STRING = "WorldEnvironment";

		virtual Registerable* instance() override
		{
			WorldEnvironment* clone = new WorldEnvironment(*this);
			clone->id = UIDGenerator::get_new_uid();
			return clone;
		}

		std::shared_ptr<Colour> clear_colour;
		BackgroundMode background_mode;
		ToneMapper tone_mapper;
		std::shared_ptr<Shader> shader;

		virtual void deserialize(YAML::Node yaml_node);
	private:
	};
}