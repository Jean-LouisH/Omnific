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
#include "scene/components/gui_element.hpp"
#include "scene/components/label.hpp"

namespace Omnific
{
	class OMNIFIC_ENGINE_API Button : public GUIElement
	{
	public:
		Button()
		{
			this->type = TYPE_STRING;
			this->is_clickable = true;
			this->is_highlightable = true;
			this->gui_label = std::shared_ptr<Label>(new Label());
			this->button_space_from_text = glm::vec2(20.0);
		};
		static constexpr const char* TYPE_STRING = "Button";

		virtual Registerable* instance() override
		{
			Button* clone = new Button(*this);
			clone->id = UIDGenerator::get_new_uid();
			return clone;
		}

		std::shared_ptr<Label> gui_label;
		glm::vec2 button_space_from_text = glm::vec2(0.0);

		virtual void deserialize(YAML::Node yaml_node);
		virtual void update_image() override;
	private:

	};
}