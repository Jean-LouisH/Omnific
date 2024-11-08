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

#include "gui.hpp"
#include <foundations/singletons/platform/platform.hpp>
#include <foundations/singletons/uid_generator.hpp>

void Omnific::GUI::deserialize(YAML::Node yaml_node)
{
	Model::deserialize(yaml_node);

	for (YAML::const_iterator it3 = yaml_node.begin(); it3 != yaml_node.end(); ++it3)
	{
		if (it3->first.as<std::string>() == "follow_target_entity_name")
		{
			this->follow_target_entity_name = it3->second.as<std::string>();
			this->is_following_entity = true;
		}
		else if (it3->first.as<std::string>() == "follow_offset")
		{
			this->follow_offset.x = it3->second[0].as<double>();
			this->follow_offset.y = it3->second[1].as<double>();
		}
		else
		{
			this->root_element = this->deserialize_gui_element(yaml_node);
		}
	}

	this->update_image();
}

std::shared_ptr<Omnific::Image> Omnific::GUIElement::get_image()
{
	return this->image;
}

void Omnific::GUIColour::update_image()
{
	this->image = std::shared_ptr<Image>(new Image(this->target_background_colour, this->dimensions.x, this->dimensions.y));
}

void Omnific::GUIButton::update_image()
{
	/* The border of the button wraps around the text it contains by an offset. */
	this->gui_text->update_image();
	this->dimensions = this->gui_text->dimensions + this->button_space_from_text;
	this->image = std::shared_ptr<Image>(new Image(this->target_background_colour, this->dimensions.x, this->dimensions.y));
}

void Omnific::GUIPanel::update_image()
{
	/* Determine if Widgets take up more space than the Panel dimensions. */
	glm::vec2 maximum_widget_side_positions = glm::vec2(0.0);

	for (auto gui_element : this->gui_elements)
	{
		glm::vec2 gui_widget_side_positions = gui_element->position + gui_element->dimensions;
		if (gui_widget_side_positions.x > maximum_widget_side_positions.x)
			maximum_widget_side_positions.x = gui_widget_side_positions.x;
		if (gui_widget_side_positions.y > maximum_widget_side_positions.y)
			maximum_widget_side_positions.y = gui_widget_side_positions.y;
	}

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

	this->image = std::shared_ptr<Image>(new Image(this->target_background_colour, this->dimensions.x, this->dimensions.y));
}

void Omnific::GUIPanel::add_gui_element(std::shared_ptr<GUIElement> gui_element)
{
	this->gui_elements.push_back(gui_element);
	//Position the element if necessary.
	this->update_image();
}

void Omnific::GUILabel::set_text(std::string text)
{
	if (text != this->text)
	{
		this->text = text;
		this->update_image();
	}
}

void Omnific::GUILabel::set_font(std::shared_ptr<Omnific::Font> font)
{
	this->font = font;
	this->update_image();
}

void Omnific::GUILabel::set_colour(std::shared_ptr<Colour> colour)
{
	this->colour = colour;
	this->update_image();
}

void Omnific::GUILabel::update_image()
{
	if (this->font != nullptr && this->colour != nullptr)
	{
		if (this->font->get_sdl_ttf_font() != nullptr)
		{
			this->image = std::shared_ptr<Image>(new Image(this->text, this->font, this->colour, this->wrap_length));
			this->dimensions = this->image->get_dimensions();
		}
	}
}

std::shared_ptr<Omnific::GUIElement> Omnific::GUI::deserialize_gui_element(YAML::Node yaml_node)
{
	std::shared_ptr<GUIElement> gui_element;

	for (YAML::const_iterator it5 = yaml_node.begin(); it5 != yaml_node.end(); ++it5)
	{
		if (it5->first.as<std::string>() == "GUIPanel")
		{
			std::shared_ptr<GUIPanel> gui_panel(new GUIPanel());

			for (YAML::const_iterator it6 = it5->second.begin(); it6 != it5->second.end(); ++it6)
			{
				if (it6->first.as<std::string>() == "name")
				{
					gui_panel->name = it6->second.as<std::string>();
				}
				else if (it6->first.as<std::string>() == "position")
				{
					gui_panel->position.x = it6->second[0].as<double>();
					gui_panel->position.y = it6->second[1].as<double>();
				}
				else if (it6->first.as<std::string>() == "dimensions")
				{
					gui_panel->dimensions.x = it6->second[0].as<double>();
					gui_panel->dimensions.y = it6->second[1].as<double>();
				}
				else if (it6->first.as<std::string>() == "colour")
				{
					gui_panel->target_background_colour = std::shared_ptr<Colour>(new Colour(it6->second.as<std::string>()));
				}
				else if (it6->first.as<std::string>() == "image")
				{
					;
				}
				else
				{
					std::shared_ptr<GUIElement> gui_element = this->deserialize_gui_element(it5->second);
					if (gui_element != nullptr)
					{
						gui_panel->add_gui_element(gui_element);
					}
				}
			}
			gui_panel->update_image();
			gui_element = std::dynamic_pointer_cast<GUIElement>(gui_panel);
		}
		else if (it5->first.as<std::string>() == "GUIMenuBar")
		{
			std::shared_ptr<GUIMenuBar> gui_menu_bar(new GUIMenuBar());

			for (YAML::const_iterator it6 = it5->second.begin(); it6 != it5->second.end(); ++it6)
			{
				if (it6->first.as<std::string>() == "")
				{

				}
			}

			gui_element = std::dynamic_pointer_cast<GUIElement>(gui_menu_bar);
		}
		else if (it5->first.as<std::string>() == "GUIContextMenu")
		{
			std::shared_ptr<GUIContextMenu> gui_context_menu(new GUIContextMenu());

			for (YAML::const_iterator it6 = it5->second.begin(); it6 != it5->second.end(); ++it6)
			{
				if (it6->first.as<std::string>() == "")
				{

				}
			}

			gui_element = std::dynamic_pointer_cast<GUIElement>(gui_context_menu);
		}
		else if (it5->first.as<std::string>() == "GUIColour")
		{
			std::shared_ptr<GUIColour> gui_colour(new GUIColour());

			for (YAML::const_iterator it6 = it5->second.begin(); it6 != it5->second.end(); ++it6)
			{
				if (it6->first.as<std::string>() == "colour")
				{
					gui_colour->target_background_colour = std::shared_ptr<Colour>(new Colour(it6->second.as<std::string>()));
				}
			}

			gui_colour->update_image();
			gui_element = std::dynamic_pointer_cast<GUIElement>(gui_colour);
		}
		else if (it5->first.as<std::string>() == "GUIButton")
		{
			std::shared_ptr<GUIButton> gui_button(new GUIButton());

			gui_button->is_clickable = true;
			gui_button->is_highlightable = true;

			for (YAML::const_iterator it6 = it5->second.begin(); it6 != it5->second.end(); ++it6)
			{
				if (it6->first.as<std::string>() == "is_x_stretched_to_panel")
				{
					gui_button->is_xstretched_to_panel = it6->second.as<bool>();
				}
				else if (it6->first.as<std::string>() == "is_y_stretched_to_panel")
				{
					gui_button->is_ystretched_to_panel = it6->second.as<bool>();
				}
				else if (it6->first.as<std::string>() == "default_dimensions")
				{
					gui_button->dimensions.x = it6->second[0].as<double>();
					gui_button->dimensions.y = it6->second[1].as<double>();
				}
				else if (it6->first.as<std::string>() == "position")
				{
					gui_button->position.x = it6->second[0].as<double>();
					gui_button->position.y = it6->second[1].as<double>();
				}
				else if (it6->first.as<std::string>() == "text")
				{
					gui_button->gui_text->text = it6->second.as<std::string>();
				}
				else if (it6->first.as<std::string>() == "font")
				{
					std::shared_ptr<Omnific::Font> font = Platform::get_file_access().load_resource_by_type<Font>(it6->second[0].as<std::string>());
					*font = Font(font->get_name(), it6->second[1].as<int>());
					gui_button->gui_text->font = font;
				}
				else if (it6->first.as<std::string>() == "text_colour")
				{
					gui_button->gui_text->colour = std::shared_ptr<Colour>(new Colour(it6->second.as<std::string>()));
				}
				else if (it6->first.as<std::string>() == "colour")
				{
					gui_button->target_background_colour = std::shared_ptr<Colour>(new Colour(it6->second.as<std::string>()));
				}
				else if (it6->first.as<std::string>() == "image")
				{
					;
				}
			}
			gui_button->update_image();
			gui_element = std::dynamic_pointer_cast<GUIElement>(gui_button);
		}
		else if (it5->first.as<std::string>() == "GUIToggleButton")
		{
			std::shared_ptr<GUIToggleButton> gui_toggle_button(new GUIToggleButton());

			for (YAML::const_iterator it6 = it5->second.begin(); it6 != it5->second.end(); ++it6)
			{
				if (it6->first.as<std::string>() == "")
				{

				}
			}

			gui_element = std::dynamic_pointer_cast<GUIElement>(gui_toggle_button);
		}
		else if (it5->first.as<std::string>() == "GUIList")
		{
			std::shared_ptr<GUIList> gui_list(new GUIList());

			for (YAML::const_iterator it6 = it5->second.begin(); it6 != it5->second.end(); ++it6)
			{
				if (it6->first.as<std::string>() == "")
				{

				}
			}

			gui_element = std::dynamic_pointer_cast<GUIElement>(gui_list);
		}
		else if (it5->first.as<std::string>() == "GUITree")
		{
			std::shared_ptr<GUITree> gui_tree(new GUITree());

			for (YAML::const_iterator it6 = it5->second.begin(); it6 != it5->second.end(); ++it6)
			{
				if (it6->first.as<std::string>() == "")
				{

				}
			}

			gui_element = std::dynamic_pointer_cast<GUIElement>(gui_tree);
		}
		else if (it5->first.as<std::string>() == "GUILabel")
		{
			std::shared_ptr<GUILabel> gui_text(new GUILabel());

			for (YAML::const_iterator it6 = it5->second.begin(); it6 != it5->second.end(); ++it6)
			{
				if (it6->first.as<std::string>() == "is_clickable")
				{
					gui_text->is_clickable = it6->second.as<bool>();
				}
				if (it6->first.as<std::string>() == "is_highlightable")
				{
					gui_text->is_highlightable = it6->second.as<bool>();
				}
				if (it6->first.as<std::string>() == "is_x_stretched_to_panel")
				{
					gui_text->is_xstretched_to_panel = it6->second.as<bool>();
				}
				else if (it6->first.as<std::string>() == "is_y_stretched_to_panel")
				{
					gui_text->is_ystretched_to_panel = it6->second.as<bool>();
				}
				else if (it6->first.as<std::string>() == "position")
				{
					gui_text->position.x = it6->second[0].as<double>();
					gui_text->position.y = it6->second[1].as<double>();
				}
				else if (it6->first.as<std::string>() == "text")
				{
					gui_text->text = it6->second.as<std::string>();
				}
				else if (it6->first.as<std::string>() == "font")
				{
					std::shared_ptr<Omnific::Font> font = Platform::get_file_access().load_resource_by_type<Font>(it6->second[0].as<std::string>());
					*font = Font(font->get_name(), it6->second[1].as<int>());
					gui_text->font = font;
				}
				else if (it6->first.as<std::string>() == "colour")
				{
					gui_text->colour = std::shared_ptr<Colour>(new Colour(it6->second.as<std::string>()));
				}
				else if (it6->first.as<std::string>() == "wrap_length")
				{
					gui_text->wrap_length = it6->second.as<int>();
				}
			}
			gui_text->update_image();
			gui_element = std::dynamic_pointer_cast<GUIElement>(gui_text);
		}
		else if (it5->first.as<std::string>() == "GUITiles")
		{
			std::shared_ptr<GUITiles> gui_tiles(new GUITiles());

			for (YAML::const_iterator it6 = it5->second.begin(); it6 != it5->second.end(); ++it6)
			{
				if (it6->first.as<std::string>() == "")
				{

				}
			}

			gui_element = std::dynamic_pointer_cast<GUIElement>(gui_tiles);
		}
		else if (it5->first.as<std::string>() == "GUIListBox")
		{
			std::shared_ptr<GUIList> gui_list_box(new GUIList());

			for (YAML::const_iterator it6 = it5->second.begin(); it6 != it5->second.end(); ++it6)
			{
				if (it6->first.as<std::string>() == "")
				{

				}
			}

			gui_element = std::dynamic_pointer_cast<GUIElement>(gui_list_box);
		}
		else if (it5->first.as<std::string>() == "GUISpinner")
		{
			std::shared_ptr<GUISpinner> gui_spinner(new GUISpinner());

			for (YAML::const_iterator it6 = it5->second.begin(); it6 != it5->second.end(); ++it6)
			{
				if (it6->first.as<std::string>() == "")
				{

				}
			}

			gui_element = std::dynamic_pointer_cast<GUIElement>(gui_spinner);
		}
		else if (it5->first.as<std::string>() == "GUIDropDownList")
		{
			std::shared_ptr<GUIDropDownList> gui_drop_down_list(new GUIDropDownList());

			for (YAML::const_iterator it6 = it5->second.begin(); it6 != it5->second.end(); ++it6)
			{
				if (it6->first.as<std::string>() == "")
				{

				}
			}

			gui_element = std::dynamic_pointer_cast<GUIElement>(gui_drop_down_list);
		}
		else if (it5->first.as<std::string>() == "GUISlider")
		{
			std::shared_ptr<GUISlider> gui_slider(new GUISlider());

			for (YAML::const_iterator it6 = it5->second.begin(); it6 != it5->second.end(); ++it6)
			{
				if (it6->first.as<std::string>() == "")
				{

				}
			}

			gui_element = std::dynamic_pointer_cast<GUIElement>(gui_slider);
		}
		else if (it5->first.as<std::string>() == "GUILine")
		{
			std::shared_ptr<GUILine> gui_line(new GUILine());

			for (YAML::const_iterator it6 = it5->second.begin(); it6 != it5->second.end(); ++it6)
			{
				if (it6->first.as<std::string>() == "")
				{

				}
			}

			gui_element = std::dynamic_pointer_cast<GUIElement>(gui_line);
		}
		else if (it5->first.as<std::string>() == "GUITreeView")
		{
			std::shared_ptr<GUITreeView> gui_tree_view(new GUITreeView());

			for (YAML::const_iterator it6 = it5->second.begin(); it6 != it5->second.end(); ++it6)
			{

			}

			gui_element = std::dynamic_pointer_cast<GUIElement>(gui_tree_view);
		}
	}

	return gui_element;
}

void Omnific::GUI::update_image()
{
	this->set_to_image(this->root_element->get_image());
}

void Omnific::GUI::set_as_text(std::string text)
{
	if (this->root_element == nullptr)
	{
		this->root_element = std::shared_ptr<GUIElement>(new GUIElement());
	}

	if (root_element->gui_element_type == GUILabel::TYPE_STRING)
	{
		std::shared_ptr<GUILabel> gui_label = std::dynamic_pointer_cast<GUILabel>(root_element);
		gui_label->set_text(text);
	}
	else
	{
		this->element_cache.clear();
		std::shared_ptr<GUILabel> gui_label = std::shared_ptr<GUILabel>(new GUILabel());
		gui_label->set_colour(std::shared_ptr<Colour>(new Colour("#CCCCCC")));
		gui_label->set_font(std::shared_ptr<Font>(new Font()));
		gui_label->set_text(text);
		this->root_element = gui_label;
	}

	this->update_image();
}

std::shared_ptr<Omnific::GUIElement> Omnific::GUI::get_element(std::string gui_element_name)
{
	std::shared_ptr<GUIElement> gui_element;

	if (this->element_cache.count(gui_element_name))
		gui_element = this->element_cache.at(gui_element_name);

	return gui_element;
}

std::shared_ptr<Omnific::GUIElement> Omnific::GUI::get_root_element()
{
	return this->root_element;
}