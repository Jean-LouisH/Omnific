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

#include "gui_element.hpp"


void Omnific::GUIElement::deserialize(YAML::Node yaml_node)
{
	Renderable::deserialize(yaml_node);

	for (YAML::const_iterator it = yaml_node.begin(); it != yaml_node.end(); ++it)
	{
		if (it->first.as<std::string>() == "pivot")
		{
			if (it->second.as<std::string>() == "top_left")
				this->pivot = GUIElement::PivotPoint::TOP_LEFT;
			else if (it->second.as<std::string>() == "top_centre")
				this->pivot = GUIElement::PivotPoint::TOP_CENTRE;
			else if (it->second.as<std::string>() == "top_right")
				this->pivot = GUIElement::PivotPoint::TOP_RIGHT;
			else if (it->second.as<std::string>() == "centre_left")
				this->pivot = GUIElement::PivotPoint::CENTRE_LEFT;
			else if (it->second.as<std::string>() == "centre")
				this->pivot = GUIElement::PivotPoint::CENTRE;
			else if (it->second.as<std::string>() == "centre_right")
				this->pivot = GUIElement::PivotPoint::CENTRE_RIGHT;
			else if (it->second.as<std::string>() == "bottom_left")
				this->pivot = GUIElement::PivotPoint::BOTTOM_LEFT;
			else if (it->second.as<std::string>() == "bottom_centre")
				this->pivot = GUIElement::PivotPoint::BOTTOM_CENTRE;
			else if (it->second.as<std::string>() == "bottom_right")
				this->pivot = GUIElement::PivotPoint::BOTTOM_RIGHT;
		}
		else if (it->first.as<std::string>() == "anchor_pivot")
		{
			if (it->second.as<std::string>() == "top_left")
				this->anchor_pivot = GUIElement::PivotPoint::TOP_LEFT;
			else if (it->second.as<std::string>() == "top_centre")
				this->anchor_pivot = GUIElement::PivotPoint::TOP_CENTRE;
			else if (it->second.as<std::string>() == "top_right")
				this->anchor_pivot = GUIElement::PivotPoint::TOP_RIGHT;
			else if (it->second.as<std::string>() == "centre_left")
				this->anchor_pivot = GUIElement::PivotPoint::CENTRE_LEFT;
			else if (it->second.as<std::string>() == "centre")
				this->anchor_pivot = GUIElement::PivotPoint::CENTRE;
			else if (it->second.as<std::string>() == "centre_right")
				this->anchor_pivot = GUIElement::PivotPoint::CENTRE_RIGHT;
			else if (it->second.as<std::string>() == "bottom_left")
				this->anchor_pivot = GUIElement::PivotPoint::BOTTOM_LEFT;
			else if (it->second.as<std::string>() == "bottom_centre")
				this->anchor_pivot = GUIElement::PivotPoint::BOTTOM_CENTRE;
			else if (it->second.as<std::string>() == "bottom_right")
				this->anchor_pivot = GUIElement::PivotPoint::BOTTOM_RIGHT;
		}
		else if (it->first.as<std::string>() == "margin")
		{
			this->margin.x = it->second[0].as<double>();
			this->margin.y = it->second[1].as<double>();
		}
		else if (it->first.as<std::string>() == "is_clickable")
		{
			this->is_clickable = it->second.as<bool>();
		}
		else if (it->first.as<std::string>() == "is_highlightable")
		{
			this->is_highlightable = it->second.as<bool>();
		}
		else if (it->first.as<std::string>() == "is_x_stretched_to_panel")
		{
			this->is_xstretched_to_panel = it->second.as<bool>();
		}
		else if (it->first.as<std::string>() == "is_y_stretched_to_panel")
		{
			this->is_ystretched_to_panel = it->second.as<bool>();
		}
	}
}

void Omnific::GUIElement::update_image()
{

}

bool Omnific::GUIElement::get_is_in_focus()
{
	return this->is_hovered_in_focus;
}

bool Omnific::GUIElement::get_is_selected()
{
	return this->is_selected;
}

glm::vec2 Omnific::GUIElement::get_position_pivot_offset()
{
	return this->position_pivot_offset;
}

void Omnific::GUIElement::highlight_on_input()
{
	if (this->is_highlightable)
	{
		if (this->is_clicked)
		{
			this->target_current_colour = this->target_clicked_colour;
		}
		else if (this->is_hovered_in_focus)
		{
			this->target_current_colour  = this->target_highlight_colour;
		}
		else
		{
			this->target_current_colour  = this->target_default_background_colour;
		}
	}
}