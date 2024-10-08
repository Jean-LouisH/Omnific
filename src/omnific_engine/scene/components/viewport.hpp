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

#include <foundations/aliases.hpp>
#include <foundations/constants.hpp>
#include "scene/components/component.hpp"
#include <foundations/singletons/platform/platform.hpp>
#include <glm.hpp>

namespace Omnific
{
	class OMNIFIC_ENGINE_API Viewport : public Component
	{
	public:
		Viewport()
		{
			this->type = TYPE_STRING;
			this->dimensions = {800, 640};
		};
		static constexpr const char* TYPE_STRING = "Viewport";

		virtual Registerable* instance() override
		{
			Viewport* clone = new Viewport(*this);
			clone->id = UIDGenerator::get_new_uid();
			return clone;
		}
		virtual void deserialize(YAML::Node yaml_node);
		std::string get_camera_entity_name();
		glm::vec2 get_dimensions();
		void set_camera_entity_name(std::string camera_entity_name);
		void set_dimensions(uint32_t width, uint32_t height);
	private:
		glm::vec2 dimensions;
		std::string camera_entity_name;
	};
}