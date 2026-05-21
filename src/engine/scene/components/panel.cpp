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

#include "panel.hpp"

void Omnific::Panel::deserialize(YAML::Node yaml_node)
{
	GUIElement::deserialize(yaml_node);

	for (YAML::const_iterator it3 = yaml_node.begin(); it3 != yaml_node.end(); ++it3)
	{
		if (it3->first.as<std::string>() == "")
		{

		}
	}

	this->update_image();
}

void Omnific::Panel::update_image()
{
	if (!this->is_hidden())
	{
		/* Determine if Widgets take up more space than the Panel dimensions. */
		glm::vec2 maximum_widget_side_positions = glm::vec2(0.0);

		// for (auto gui_element : this->gui_elements)
		// {
		// 	glm::vec2 gui_widget_side_positions = gui_element->position + gui_element->dimensions;
		// 	if (gui_widget_side_positions.x > maximum_widget_side_positions.x)
		// 		maximum_widget_side_positions.x = gui_widget_side_positions.x;
		// 	if (gui_widget_side_positions.y > maximum_widget_side_positions.y)
		// 		maximum_widget_side_positions.y = gui_widget_side_positions.y;
		// }

		/* If so, then create the scrollbars for the sides of the panel. */
		if (maximum_widget_side_positions.x > this->dimensions.x)
		{
			float percentage_of_total_horizontal_space = (float)this->dimensions.x / (float)maximum_widget_side_positions.x;
			glm::vec2 horizontal_scrollbar_thumb_dimensions = glm::vec2(percentage_of_total_horizontal_space * this->dimensions.x, this->scrollbar_thickness);
			this->horizontal_scrollbar_thumb_image = std::shared_ptr<Image>(new Image(this->target_scrollbar_thumb_colour, horizontal_scrollbar_thumb_dimensions.x, horizontal_scrollbar_thumb_dimensions.y));
		}

		if (maximum_widget_side_positions.y > this->dimensions.y)
		{
			float percentage_of_total_vertical_space = (float)this->dimensions.y / (float)maximum_widget_side_positions.y;
			glm::vec2 vertical_scrollbar_thumb_dimensions = glm::vec2(this->scrollbar_thickness, percentage_of_total_vertical_space * this->dimensions.y);
			this->vertical_scrollbar_thumb_image = std::shared_ptr<Image>(new Image(this->target_scrollbar_thumb_colour, vertical_scrollbar_thumb_dimensions.x, vertical_scrollbar_thumb_dimensions.y));
		}

		this->set_to_image(std::shared_ptr<Image>(new Image(this->background_colour, this->dimensions.x, this->dimensions.y)));
			
	}
}

void Omnific::Panel::add_gui_element(std::shared_ptr<GUIElement> gui_element)
{
	this->gui_elements.push_back(gui_element);
	//Position the element if necessary.
	this->update_image();
}