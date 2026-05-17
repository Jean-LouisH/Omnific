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

namespace Omnific
{
	class OMNIFIC_ENGINE_API Panel : public GUIElement
	{
	public:
		Panel()
		{
			this->type = TYPE_STRING;
			this->target_default_background_colour = std::shared_ptr<Colour>(new Colour(0.8, 0.8, 0.8, 1.0));
			this->target_scrollbar_colour = std::shared_ptr<Colour>(new Colour("#222222"));
			this->target_scrollbar_thumb_colour = std::shared_ptr<Colour>(new Colour("#3b3b3b"));
			this->target_scrollbar_arrow_colour = std::shared_ptr<Colour>(new Colour("#3b3b3b"));
			this->scrollbar_thickness = 5;
		};
		static constexpr const char* TYPE_STRING = "Panel";

		virtual Registerable* instance() override
		{
			Panel* clone = new Panel(*this);
			clone->id = UIDGenerator::get_new_uid();
			return clone;
		}

		std::vector<std::shared_ptr<GUIElement>> gui_elements;
		std::shared_ptr<Colour> target_scrollbar_colour;
		std::shared_ptr<Colour> target_scrollbar_thumb_colour;
		std::shared_ptr<Colour> target_scrollbar_arrow_colour;
		uint32_t scrollbar_thickness;

		/* Denoted in the image pixel position of the top view of the panel
		where an increasing y value travels from top to bottom. */
		uint32_t vertical_slider_position = 0;
		uint32_t horizontal_slider_position = 0;

		std::shared_ptr<Image> scrollbar_arrow_image;
		std::shared_ptr<Image> scrollbar_image;
		std::shared_ptr<Image> vertical_scrollbar_thumb_image;
		std::shared_ptr<Image> horizontal_scrollbar_thumb_image;

		void add_gui_element(std::shared_ptr<GUIElement> gui_element);

		virtual void deserialize(YAML::Node yaml_node);
		virtual void update_image() override;
	private:

	};
}