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
#include "foundations/constants.hpp"
#include "scene/components/component.hpp"
#include "scene/components/model.hpp"
#include <foundations/colour.hpp>


namespace Omnific
{
	class OMNIFIC_ENGINE_API Line : public Model
	{
	public:
		Line()
		{
			this->type = TYPE_STRING;
		};
		static constexpr const char* TYPE_STRING = "Line";

		virtual Registerable* instance() override
		{
			Line* clone = new Line(*this);
			clone->id = UIDGenerator::get_new_uid();
			return clone;
		}
		virtual void deserialize(YAML::Node yaml_node);
		void generate_from_points(glm::vec3 point_a, glm::vec3 point_b, std::shared_ptr<Colour> colour);
		void generate_from_point_list(std::vector<glm::vec3> points, std::shared_ptr<Colour> colour);
	private:
	};
}