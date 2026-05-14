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
	Renderable::deserialize(yaml_node);

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
		else if (it3->first.as<std::string>() == "is_2d_override")
		{
			this->is_2d_override = it3->second.as<bool>();
		}
		else
		{
			this->root_element = this->deserialize_gui_element(yaml_node);
		}
	}

	this->update_image();
}

void Omnific::GUI::Element::deserialize_common_properties(YAML::Node yaml_node)
{
	for (YAML::const_iterator it = yaml_node.begin(); it != yaml_node.end(); ++it)
	{
		if (it->first.as<std::string>() == "name")
		{
			this->name = it->second.as<std::string>();
		}
		else if (it->first.as<std::string>() == "position")
		{
			this->position.x = it->second[0].as<double>();
			this->position.y = it->second[1].as<double>();
		}
		else if (it->first.as<std::string>() == "dimensions")
		{
			this->dimensions.x = it->second[0].as<double>();
			this->dimensions.y = it->second[1].as<double>();
		}
		else if (it->first.as<std::string>() == "pivot")
		{
			if (it->second.as<std::string>() == "top_left")
				this->pivot = GUI::Element::PivotPoint::TOP_LEFT;
			else if (it->second.as<std::string>() == "top_centre")
				this->pivot = GUI::Element::PivotPoint::TOP_CENTRE;
			else if (it->second.as<std::string>() == "top_right")
				this->pivot = GUI::Element::PivotPoint::TOP_RIGHT;
			else if (it->second.as<std::string>() == "centre_left")
				this->pivot = GUI::Element::PivotPoint::CENTRE_LEFT;
			else if (it->second.as<std::string>() == "centre")
				this->pivot = GUI::Element::PivotPoint::CENTRE;
			else if (it->second.as<std::string>() == "centre_right")
				this->pivot = GUI::Element::PivotPoint::CENTRE_RIGHT;
			else if (it->second.as<std::string>() == "bottom_left")
				this->pivot = GUI::Element::PivotPoint::BOTTOM_LEFT;
			else if (it->second.as<std::string>() == "bottom_centre")
				this->pivot = GUI::Element::PivotPoint::BOTTOM_CENTRE;
			else if (it->second.as<std::string>() == "bottom_right")
				this->pivot = GUI::Element::PivotPoint::BOTTOM_RIGHT;
		}
		else if (it->first.as<std::string>() == "anchor_pivot")
		{
			if (it->second.as<std::string>() == "top_left")
				this->anchor_pivot = GUI::Element::PivotPoint::TOP_LEFT;
			else if (it->second.as<std::string>() == "top_centre")
				this->anchor_pivot = GUI::Element::PivotPoint::TOP_CENTRE;
			else if (it->second.as<std::string>() == "top_right")
				this->anchor_pivot = GUI::Element::PivotPoint::TOP_RIGHT;
			else if (it->second.as<std::string>() == "centre_left")
				this->anchor_pivot = GUI::Element::PivotPoint::CENTRE_LEFT;
			else if (it->second.as<std::string>() == "centre")
				this->anchor_pivot = GUI::Element::PivotPoint::CENTRE;
			else if (it->second.as<std::string>() == "centre_right")
				this->anchor_pivot = GUI::Element::PivotPoint::CENTRE_RIGHT;
			else if (it->second.as<std::string>() == "bottom_left")
				this->anchor_pivot = GUI::Element::PivotPoint::BOTTOM_LEFT;
			else if (it->second.as<std::string>() == "bottom_centre")
				this->anchor_pivot = GUI::Element::PivotPoint::BOTTOM_CENTRE;
			else if (it->second.as<std::string>() == "bottom_right")
				this->anchor_pivot = GUI::Element::PivotPoint::BOTTOM_RIGHT;
		}
		else if (it->first.as<std::string>() == "margin")
		{
			this->margin.x = it->second[0].as<double>();
			this->margin.y = it->second[1].as<double>();
		}
		else if (it->first.as<std::string>() == "image")
		{
			;
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

void Omnific::GUI::Element::update_image()
{

}

bool Omnific::GUI::Element::get_is_in_focus()
{
	return this->is_hovered_in_focus;
}

bool Omnific::GUI::Element::get_is_selected()
{
	return this->is_selected;
}

glm::vec2 Omnific::GUI::Element::get_position()
{
	return this->position;
}

glm::vec2 Omnific::GUI::Element::get_dimensions()
{
	return this->dimensions;
}

glm::vec2 Omnific::GUI::Element::get_position_pivot_offset()
{
	return this->position_pivot_offset;
}

std::string Omnific::GUI::Element::get_name()
{
	return this->name;
}
	
std::string Omnific::GUI::Element::get_gui_element_type()
{
	return this->gui_element_type;
}

std::string Omnific::GUI::Element::get_parent_type()
{
	return this->parent_type;
}

std::shared_ptr<Omnific::Image> Omnific::GUI::Element::get_image()
{
	return this->image;
}

void Omnific::GUI::Element::highlight_on_input()
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

void Omnific::GUI::PlotCanvas::update_image()
{
	if (!this->is_hidden)
	{
		this->image = std::shared_ptr<Image>(new Image(this->plot_points, this->dimensions.x, this->dimensions.y, this->target_plot_background_colour, this->target_plot_colour));
	}
}

void Omnific::GUI::ColourCanvas::update_image()
{
	if (!this->is_hidden)
	{
		this->image = std::shared_ptr<Image>(new Image(this->target_default_background_colour, this->dimensions.x, this->dimensions.y));
	}
}

void Omnific::GUI::Button::update_image()
{
	if (!this->is_hidden)
	{
		this->highlight_on_input();
		
		/* The border of the button wraps around the text it contains by an offset. */
		this->dimensions = this->gui_label->get_dimensions() + this->button_space_from_text;
		std::shared_ptr<Image> base_button_image = std::shared_ptr<Image>(new Image(this->target_current_colour, this->dimensions.x, this->dimensions.y));
		std::shared_ptr<Image> gui_label_image = this->gui_label->get_image();

		Image::normal_blend(
			base_button_image->get_data(), 
			glm::vec2(0.0), 
			this->dimensions, 
			gui_label_image->get_data(), 
			this->button_space_from_text * 0.5f, 
			gui_label->get_dimensions());

		this->image = std::shared_ptr<Image>(new Image(
			base_button_image->get_data(), 
			this->dimensions.x,
			this->dimensions.y,
			base_button_image->get_bytes_per_pixel()));

	}
}

void Omnific::GUI::ButtonList::update_image()
{

}

void Omnific::GUI::ButtonList::add_item(std::string button_name)
{
	std::shared_ptr<Button> button(new Button());
	button->gui_label->set_text(button_name);
	this->list_items.push_back(button);
}

std::vector<std::shared_ptr<Omnific::GUI::Button>> Omnific::GUI::ButtonList::get_list_items()
{
	return this->list_items;
}

void Omnific::GUI::Panel::update_image()
{
	if (!this->is_hidden)
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

		this->image = std::shared_ptr<Image>(new Image(this->target_default_background_colour, this->dimensions.x, this->dimensions.y));
			
	}
}

void Omnific::GUI::Panel::add_gui_element(std::shared_ptr<Element> gui_element)
{
	this->gui_elements.push_back(gui_element);
	//Position the element if necessary.
	this->update_image();
}

void Omnific::GUI::Label::set_text(std::string text)
{
	if (text != this->text)
	{
		this->text = text;
		this->update_image();
	}
}

void Omnific::GUI::Label::set_font(std::shared_ptr<Omnific::Font> font)
{
	this->font = font;
	this->update_image();
}

void Omnific::GUI::Label::set_text_colour(std::shared_ptr<Colour> colour)
{
	this->text_colour = colour;
	this->update_image();
}

void Omnific::GUI::Label::update_image()
{
	if (!this->is_hidden && this->font != nullptr && this->text_colour != nullptr)
	{
		if (this->font->get_sdl_ttf_font() != nullptr)
		{
			this->image = std::shared_ptr<Image>(new Image(this->text, this->font, this->text_colour, this->wrap_length));
			this->dimensions = this->image->get_dimensions();
		}
	}
}

std::shared_ptr<Omnific::GUI::Element> Omnific::GUI::deserialize_gui_element(YAML::Node yaml_node)
{
	std::shared_ptr<Element> gui_element;

	for (YAML::const_iterator it5 = yaml_node.begin(); it5 != yaml_node.end(); ++it5)
	{
		if (it5->first.as<std::string>() == "Panel")
		{
			std::shared_ptr<Panel> gui_panel(new Panel());

			for (YAML::const_iterator it6 = it5->second.begin(); it6 != it5->second.end(); ++it6)
			{
				if (it6->first.as<std::string>() == "x")
				{
					;
				}
				else
				{
					std::shared_ptr<Element> gui_element = this->deserialize_gui_element(it5->second);
					if (gui_element != nullptr)
					{
						gui_panel->add_gui_element(gui_element);
					}
				}
			}
			gui_panel->deserialize_common_properties(it5->second);
			gui_panel->update_image();
			gui_element = std::dynamic_pointer_cast<Element>(gui_panel);
		}
		else if (it5->first.as<std::string>() == "ImageCanvas")
		{
			std::shared_ptr<ImageCanvas> gui_image_canvas(new ImageCanvas());

			for (YAML::const_iterator it6 = it5->second.begin(); it6 != it5->second.end(); ++it6)
			{
				if (it6->first.as<std::string>() == "")
				{

				}
			}

			gui_image_canvas->deserialize_common_properties(it5->second);
			gui_element = std::dynamic_pointer_cast<Element>(gui_image_canvas);
		}
		else if (it5->first.as<std::string>() == "PlotCanvas")
		{
			std::shared_ptr<PlotCanvas> gui_plot_canvas(new PlotCanvas());

			for (YAML::const_iterator it6 = it5->second.begin(); it6 != it5->second.end(); ++it6)
			{
				if (it6->first.as<std::string>() == "")
				{

				}
			}

			gui_plot_canvas->deserialize_common_properties(it5->second);
			gui_element = std::dynamic_pointer_cast<Element>(gui_plot_canvas);
		}
		else if (it5->first.as<std::string>() == "MenuBar")
		{
			std::shared_ptr<MenuBar> gui_menu_bar(new MenuBar());

			for (YAML::const_iterator it6 = it5->second.begin(); it6 != it5->second.end(); ++it6)
			{
				if (it6->first.as<std::string>() == "")
				{

				}
			}

			gui_menu_bar->deserialize_common_properties(it5->second);
			gui_element = std::dynamic_pointer_cast<Element>(gui_menu_bar);
		}
		else if (it5->first.as<std::string>() == "ContextMenu")
		{
			std::shared_ptr<ContextMenu> gui_context_menu(new ContextMenu());

			for (YAML::const_iterator it6 = it5->second.begin(); it6 != it5->second.end(); ++it6)
			{
				if (it6->first.as<std::string>() == "")
				{

				}
			}

			gui_context_menu->deserialize_common_properties(it5->second);
			gui_element = std::dynamic_pointer_cast<Element>(gui_context_menu);
		}
		else if (it5->first.as<std::string>() == "ColourCanvas")
		{
			std::shared_ptr<ColourCanvas> gui_colour(new ColourCanvas());

			for (YAML::const_iterator it6 = it5->second.begin(); it6 != it5->second.end(); ++it6)
			{
				if (it6->first.as<std::string>() == "")
				{

				}
			}

			gui_colour->deserialize_common_properties(it5->second);
			gui_colour->update_image();
			gui_element = std::dynamic_pointer_cast<Element>(gui_colour);
		}
		else if (it5->first.as<std::string>() == "Button")
		{
			std::shared_ptr<Button> gui_button(new Button());

			gui_button->gui_label = std::shared_ptr<Label>(new Label());

			for (YAML::const_iterator it6 = it5->second.begin(); it6 != it5->second.end(); ++it6)
			{
				if (it6->first.as<std::string>() == "text")
				{
					gui_button->gui_label->text = it6->second.as<std::string>();
				}
				else if (it6->first.as<std::string>() == "font")
				{
					std::shared_ptr<Omnific::Font> font = Platform::get_file_access().load_resource_by_type<Font>(it6->second[0].as<std::string>());
					*font = Font(font->get_name(), it6->second[1].as<int>());
					gui_button->gui_label->font = font;
				}
				else if (it6->first.as<std::string>() == "text_colour")
				{
					gui_button->gui_label->text_colour = std::shared_ptr<Colour>(new Colour(it6->second.as<std::string>()));
				}
			}
			gui_button->deserialize_common_properties(it5->second);
			gui_button->gui_label->update_image();
			gui_button->update_image();
			gui_element = std::dynamic_pointer_cast<Element>(gui_button);
		}
		else if (it5->first.as<std::string>() == "ToggleButton")
		{
			std::shared_ptr<ToggleButton> gui_toggle_button(new ToggleButton());

			for (YAML::const_iterator it6 = it5->second.begin(); it6 != it5->second.end(); ++it6)
			{
				if (it6->first.as<std::string>() == "")
				{

				}
			}

			gui_toggle_button->deserialize_common_properties(it5->second);
			gui_element = std::dynamic_pointer_cast<Element>(gui_toggle_button);
		}
		else if (it5->first.as<std::string>() == "ButtonList")
		{
			std::shared_ptr<ButtonList> gui_list(new ButtonList());

			for (YAML::const_iterator it6 = it5->second.begin(); it6 != it5->second.end(); ++it6)
			{
				if (it6->first.as<std::string>() == "")
				{

				}
			}

			gui_list->deserialize_common_properties(it5->second);
			gui_element = std::dynamic_pointer_cast<Element>(gui_list);
		}
		else if (it5->first.as<std::string>() == "ButtonTree")
		{
			std::shared_ptr<ButtonTree> gui_tree(new ButtonTree());

			for (YAML::const_iterator it6 = it5->second.begin(); it6 != it5->second.end(); ++it6)
			{
				if (it6->first.as<std::string>() == "")
				{

				}
			}

			gui_tree->deserialize_common_properties(it5->second);
			gui_element = std::dynamic_pointer_cast<Element>(gui_tree);
		}
		else if (it5->first.as<std::string>() == "Label")
		{
			std::shared_ptr<Label> gui_label(new Label());

			for (YAML::const_iterator it6 = it5->second.begin(); it6 != it5->second.end(); ++it6)
			{
				if (it6->first.as<std::string>() == "text")
				{
					gui_label->text = it6->second.as<std::string>();
				}
				else if (it6->first.as<std::string>() == "font")
				{
					std::shared_ptr<Omnific::Font> font = Platform::get_file_access().load_resource_by_type<Font>(it6->second[0].as<std::string>());
					*font = Font(font->get_name(), it6->second[1].as<int>());
					gui_label->font = font;
				}
				else if (it6->first.as<std::string>() == "wrap_length")
				{
					gui_label->wrap_length = it6->second.as<int>();
				}
				else if (it6->first.as<std::string>() == "text_colour")
				{
					gui_label->text_colour = std::shared_ptr<Colour>(new Colour(it6->second.as<std::string>()));
				}
			}
			gui_label->deserialize_common_properties(it5->second);
			gui_label->update_image();
			gui_element = std::dynamic_pointer_cast<Element>(gui_label);
		}
		else if (it5->first.as<std::string>() == "Tiles")
		{
			std::shared_ptr<Tiles> gui_tiles(new Tiles());

			for (YAML::const_iterator it6 = it5->second.begin(); it6 != it5->second.end(); ++it6)
			{
				if (it6->first.as<std::string>() == "")
				{

				}
			}

			gui_tiles->deserialize_common_properties(it5->second);
			gui_element = std::dynamic_pointer_cast<Element>(gui_tiles);
		}
		else if (it5->first.as<std::string>() == "ButtonList")
		{
			std::shared_ptr<ButtonList> gui_list_box(new ButtonList());

			for (YAML::const_iterator it6 = it5->second.begin(); it6 != it5->second.end(); ++it6)
			{
				if (it6->first.as<std::string>() == "")
				{

				}
			}

			gui_list_box->deserialize_common_properties(it5->second);
			gui_element = std::dynamic_pointer_cast<Element>(gui_list_box);
		}
		else if (it5->first.as<std::string>() == "Spinner")
		{
			std::shared_ptr<Spinner> gui_spinner(new Spinner());

			for (YAML::const_iterator it6 = it5->second.begin(); it6 != it5->second.end(); ++it6)
			{
				if (it6->first.as<std::string>() == "")
				{

				}
			}

			gui_spinner->deserialize_common_properties(it5->second);
			gui_element = std::dynamic_pointer_cast<Element>(gui_spinner);
		}
		else if (it5->first.as<std::string>() == "DropDownButtonList")
		{
			std::shared_ptr<DropDownButtonList> gui_drop_down_list(new DropDownButtonList());

			for (YAML::const_iterator it6 = it5->second.begin(); it6 != it5->second.end(); ++it6)
			{
				if (it6->first.as<std::string>() == "")
				{

				}
			}

			gui_drop_down_list->deserialize_common_properties(it5->second);
			gui_element = std::dynamic_pointer_cast<Element>(gui_drop_down_list);
		}
		else if (it5->first.as<std::string>() == "Slider")
		{
			std::shared_ptr<Slider> gui_slider(new Slider());

			for (YAML::const_iterator it6 = it5->second.begin(); it6 != it5->second.end(); ++it6)
			{
				if (it6->first.as<std::string>() == "")
				{

				}
			}

			gui_slider->deserialize_common_properties(it5->second);
			gui_element = std::dynamic_pointer_cast<Element>(gui_slider);
		}
		else if (it5->first.as<std::string>() == "LineElement")
		{
			std::shared_ptr<LineElement> gui_line(new LineElement());

			for (YAML::const_iterator it6 = it5->second.begin(); it6 != it5->second.end(); ++it6)
			{
				if (it6->first.as<std::string>() == "")
				{

				}
			}

			gui_line->deserialize_common_properties(it5->second);
			gui_element = std::dynamic_pointer_cast<Element>(gui_line);
		}
		else if (it5->first.as<std::string>() == "ButtonTreeView")
		{
			std::shared_ptr<ButtonTreeView> gui_tree_view(new ButtonTreeView());

			for (YAML::const_iterator it6 = it5->second.begin(); it6 != it5->second.end(); ++it6)
			{
				if (it6->first.as<std::string>() == "")
				{

				}
			}

			gui_tree_view->deserialize_common_properties(it5->second);
			gui_element = std::dynamic_pointer_cast<Element>(gui_tree_view);
		}
	}

	return gui_element;
}

void Omnific::GUI::update_image()
{
	this->set_to_image(this->root_element->get_image());
}

void Omnific::GUI::set_to_label(std::string text)
{
	if (this->root_element == nullptr)
	{
		this->root_element = std::shared_ptr<Element>(new Element());
	}

	if (root_element->gui_element_type == Label::TYPE_STRING)
	{
		std::shared_ptr<Label> gui_label = std::dynamic_pointer_cast<Label>(root_element);
		gui_label->set_text(text);
	}
	else
	{
		this->element_cache.clear();
		std::shared_ptr<Label> gui_label = std::shared_ptr<Label>(new Label());
		gui_label->set_text_colour(std::shared_ptr<Colour>(new Colour("#CCCCCC")));
		gui_label->set_font(std::shared_ptr<Font>(new Font()));
		gui_label->set_text(text);
		this->root_element = gui_label;
	}

	this->update_image();
}

void Omnific::GUI::set_to_plot(std::vector<float> plot_points, std::shared_ptr<Colour> background_colour, std::shared_ptr<Colour> plot_colour)
{
	if (this->root_element == nullptr)
	{
		this->root_element = std::shared_ptr<Element>(new Element());
	}

	std::shared_ptr<PlotCanvas> gui_plot;

	if (root_element->gui_element_type == PlotCanvas::TYPE_STRING)
	{
		gui_plot = std::dynamic_pointer_cast<PlotCanvas>(root_element);
	}
	else
	{
		this->element_cache.clear();
		gui_plot = std::shared_ptr<PlotCanvas>(new PlotCanvas());
		this->root_element = gui_plot;
	}

	gui_plot->plot_points = plot_points;
	gui_plot->target_plot_colour = plot_colour;
	gui_plot->target_plot_background_colour = background_colour;
	gui_plot->dimensions = Platform::get_window().get_window_size();
	gui_plot->update_image();
	this->update_image();
}

void Omnific::GUI::set_to_colour_canvas(std::shared_ptr<Colour> colour)
{
	// Implementation for setting colour canvas
}

void Omnific::GUI::set_to_image_canvas(std::shared_ptr<Image> image)
{
	// Implementation for setting image canvas
}

std::shared_ptr<Omnific::GUI::Element> Omnific::GUI::get_element(std::string gui_element_name)
{
	std::shared_ptr<Element> gui_element;

	if (this->element_cache.count(gui_element_name))
		gui_element = this->element_cache.at(gui_element_name);

	return gui_element;
}

std::shared_ptr<Omnific::GUI::Element> Omnific::GUI::get_root_element()
{
	return this->root_element;
}