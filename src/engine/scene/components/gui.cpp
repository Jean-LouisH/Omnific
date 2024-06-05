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

void Omnia::GUI::deserialize(YAML::Node yaml_node)
{
	for (YAML::const_iterator it3 = yaml_node.begin(); it3 != yaml_node.end(); ++it3)
	{
		Model::deserialize(yaml_node);

		if (it3->first.as<std::string>() == "GUIPanelTabGroup")
		{
			std::shared_ptr<GUIPanelTabGroup> gui_panel_tab_group(new GUIPanelTabGroup());

			for (YAML::const_iterator it4 = it3->second.begin(); it4 != it3->second.end(); ++it4)
			{
				if (it4->first.as<std::string>() == "name")
				{
					gui_panel_tab_group->name = it4->second.as<std::string>();
				}
				else if (it4->first.as<std::string>() == "position")
				{
					gui_panel_tab_group->position.x = it4->second[0].as<double>();
					gui_panel_tab_group->position.y = it4->second[1].as<double>();
				}
				else if (it4->first.as<std::string>() == "dimensions")
				{
					gui_panel_tab_group->dimensions.x = it4->second[0].as<double>();
					gui_panel_tab_group->dimensions.y = it4->second[1].as<double>();
				}
				else if (it4->first.as<std::string>() == "colour")
				{
					gui_panel_tab_group->background_colour = std::shared_ptr<Colour>(new Colour(it4->second.as<std::string>()));
				}
				else if (it4->first.as<std::string>() == "image")
				{
					;
				}
				else if (it4->first.as<std::string>() == "GUIPanel")
				{
					std::shared_ptr<GUIPanel> gui_panel(new GUIPanel());
					std::shared_ptr<GUIWidget> gui_widget;

					for (YAML::const_iterator it5 = it4->second.begin(); it5 != it4->second.end(); ++it5)
					{
						if (it5->first.as<std::string>() == "name")
						{
							gui_panel->name = it5->second.as<std::string>();
						}
						else if (it5->first.as<std::string>() == "position")
						{
							gui_panel->position.x = it5->second[0].as<double>();
							gui_panel->position.y = it5->second[1].as<double>();
						}
						else if (it5->first.as<std::string>() == "dimensions")
						{
							gui_panel->dimensions.x = it5->second[0].as<double>();
							gui_panel->dimensions.y = it5->second[1].as<double>();
						}
						else if (it5->first.as<std::string>() == "colour")
						{
							gui_panel->background_colour = std::shared_ptr<Colour>(new Colour(it5->second.as<std::string>()));
						}
						else if (it5->first.as<std::string>() == "image")
						{
							;
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

							gui_widget = std::dynamic_pointer_cast<GUIWidget>(gui_menu_bar);
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

							gui_widget = std::dynamic_pointer_cast<GUIWidget>(gui_context_menu);
						}
						else if (it5->first.as<std::string>() == "GUIColour")
						{
							std::shared_ptr<GUIColour> gui_colour(new GUIColour());

							for (YAML::const_iterator it6 = it5->second.begin(); it6 != it5->second.end(); ++it6)
							{
								if (it6->first.as<std::string>() == "colour")
								{
									gui_colour->background_colour = std::shared_ptr<Colour>(new Colour(it6->second.as<std::string>()));
								}
							}

							gui_colour->update_image();
							gui_widget = std::dynamic_pointer_cast<GUIWidget>(gui_colour);
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
									std::shared_ptr<Omnia::Font> font = Platform::get_file_access().load_asset_by_type<Font>(it6->second[0].as<std::string>());
									*font = Font(font->get_name(), it6->second[1].as<int>());
									gui_button->gui_text->font = font;
									gui_button->gui_text->size = it6->second[1].as<int>();
								}
								else if (it6->first.as<std::string>() == "text_colour")
								{
									gui_button->gui_text->colour = std::shared_ptr<Colour>(new Colour(it6->second.as<std::string>()));
								}
								else if (it6->first.as<std::string>() == "colour")
								{
									gui_button->background_colour = std::shared_ptr<Colour>(new Colour(it6->second.as<std::string>()));
								}
								else if (it6->first.as<std::string>() == "image")
								{
									;
								}
							}

							gui_button->update_image();
							gui_widget = std::dynamic_pointer_cast<GUIWidget>(gui_button);
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

							gui_widget = std::dynamic_pointer_cast<GUIWidget>(gui_toggle_button);
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

							gui_widget = std::dynamic_pointer_cast<GUIWidget>(gui_list);
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

							gui_widget = std::dynamic_pointer_cast<GUIWidget>(gui_tree);
						}
						else if (it5->first.as<std::string>() == "GUIText")
						{
							std::shared_ptr<GUIText> gui_text(new GUIText());

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
									std::shared_ptr<Omnia::Font> font = Platform::get_file_access().load_asset_by_type<Font>(it6->second[0].as<std::string>());
									*font = Font(font->get_name(), it6->second[1].as<int>());
									gui_text->font = font;
									gui_text->size = it6->second[1].as<int>();
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
							gui_widget = std::dynamic_pointer_cast<GUIWidget>(gui_text);
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

							gui_widget = std::dynamic_pointer_cast<GUIWidget>(gui_tiles);
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

							gui_widget = std::dynamic_pointer_cast<GUIWidget>(gui_list_box);
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

							gui_widget = std::dynamic_pointer_cast<GUIWidget>(gui_spinner);
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

							gui_widget = std::dynamic_pointer_cast<GUIWidget>(gui_drop_down_list);
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

							gui_widget = std::dynamic_pointer_cast<GUIWidget>(gui_slider);
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

							gui_widget = std::dynamic_pointer_cast<GUIWidget>(gui_line);
						}
						else if (it5->first.as<std::string>() == "GUITreeView")
						{
							std::shared_ptr<GUITreeView> gui_tree_view(new GUITreeView());

							for (YAML::const_iterator it6 = it5->second.begin(); it6 != it5->second.end(); ++it6)
							{

							}

							gui_widget = std::dynamic_pointer_cast<GUIWidget>(gui_tree_view);
						}

						if (gui_widget != nullptr)
							gui_panel->widgets.emplace(UIDGenerator::get_new_uid(), gui_widget);
					}

					if (gui_panel_tab_group->active_gui_panel_name == "")
						gui_panel_tab_group->active_gui_panel_name = gui_panel->name;

					gui_panel->update_image();
					gui_panel_tab_group->gui_panels.emplace(gui_panel->name, gui_panel);
				}
			}

			gui_panel_tab_group->update_image();
			this->gui_panel_tab_groups.emplace(gui_panel_tab_group->name, gui_panel_tab_group);
		}
		else if (it3->first.as<std::string>() == "follow_target_entity_name")
		{
			this->follow_target_entity_name = it3->second.as<std::string>();
			this->is_following_entity = true;
		}
		else if (it3->first.as<std::string>() == "follow_offset")
		{
			this->follow_offset.x = it3->second[0].as<double>();
			this->follow_offset.y = it3->second[1].as<double>();
		}
	}

	this->update_image();
}

void Omnia::GUIColour::update_image()
{
	this->image = std::shared_ptr<Image>(new Image(this->background_colour, this->dimensions.x, this->dimensions.y));
}

void Omnia::GUIButton::update_image()
{
	/* The border of the button wraps around the text it contains by an offset. */
	this->gui_text->update_image();
	this->dimensions = this->gui_text->dimensions + this->button_space_from_text;
	this->image = std::shared_ptr<Image>(new Image(this->background_colour, this->dimensions.x, this->dimensions.y));
}

void Omnia::GUIPanel::update_image()
{
	glm::vec2 maximum_widget_side_positions = glm::vec2(0.0);

	for (auto widget : this->widgets)
	{
		glm::vec2 gui_widget_side_positions = widget.second->position + widget.second->dimensions;
		if (gui_widget_side_positions.x > maximum_widget_side_positions.x)
			maximum_widget_side_positions.x = gui_widget_side_positions.x;
		if (gui_widget_side_positions.y > maximum_widget_side_positions.y)
			maximum_widget_side_positions.y = gui_widget_side_positions.y;
	}

	/* If Widgets take up more space than the Panel dimensions, then 
	   create the scrollbars for the sides of the panel. */
	if (maximum_widget_side_positions.x > this->dimensions.x)
	{

	}

	if (maximum_widget_side_positions.y > this->dimensions.y)
	{

	}

	this->image = std::shared_ptr<Image>(new Image(this->background_colour, this->dimensions.x, this->dimensions.y));
}

void Omnia::GUIPanelTabGroup::update_image()
{
	this->image = std::shared_ptr<Image>(new Image(this->background_colour, this->dimensions.x, this->dimensions.y));
}

void Omnia::GUIText::set_text(std::string text)
{
	if (text != this->text)
	{
		this->text = text;
		this->update_image();
	}
}

void Omnia::GUIText::set_font(std::shared_ptr<Omnia::Font> font, uint16_t size_px)
{
	this->font = font;
	this->size = size_px;
	this->update_image();
}

void Omnia::GUIText::set_font_size(uint16_t size_px)
{
	this->size = size_px;
	this->update_image();
}

void Omnia::GUIText::set_colour(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
	this->colour = std::shared_ptr<Colour>(new Colour(red, green, blue, alpha));
	this->update_image();
}

void Omnia::GUIText::update_image()
{
	if (this->font != nullptr && this->colour != nullptr)
	{
		if (this->font->get_sdlttffont() != nullptr)
		{
			this->image = std::shared_ptr<Image>(new Image(this->text, this->font, this->colour, this->wrap_length));
			this->dimensions = this->image->get_dimensions();
		}
	}
}

void Omnia::GUI::update_image()
{
	if (this->gui_panel_tab_groups.size() == 1)
	{
		for (auto gui_panel_tab_group : this->gui_panel_tab_groups)
		{
			std::shared_ptr<GUIPanel> gui_panel = gui_panel_tab_group.second->gui_panels.at(gui_panel_tab_group.second->active_gui_panel_name);

			/* If the entire GUI Component is defined by a single GUIWidget, just set the image. */
			if (gui_panel->widgets.size() == 1)
			{
				for (auto widget : gui_panel->widgets)
				{
					this->set_to_image(widget.second->image);
				}
			}
		}
	}
	else
	{
		/* Generate a single GUI Component image based on all the Widgets on the Panels. */
		std::shared_ptr<Image> updated_image;
		uint8_t* raw_pixels;
		uint8_t colour_channels = 4;
		glm::vec2 maximum_guitab_group_side_positions = glm::vec2(0.0);

		for (auto gui_panel_tab_group : this->gui_panel_tab_groups)
		{
			glm::vec2 gui_tab_group_side_positions = gui_panel_tab_group.second->position + gui_panel_tab_group.second->dimensions;
			if (gui_tab_group_side_positions.x > maximum_guitab_group_side_positions.x)
				maximum_guitab_group_side_positions.x = gui_tab_group_side_positions.x;
			if (gui_tab_group_side_positions.y > maximum_guitab_group_side_positions.y)
				maximum_guitab_group_side_positions.y = gui_tab_group_side_positions.y;
		}

		size_t raw_pixels_size = maximum_guitab_group_side_positions.x * maximum_guitab_group_side_positions.y * colour_channels;
		raw_pixels = new uint8_t[raw_pixels_size];

		/* Fill the raw pixels with empty, alpha values. */
		for (int i = 0; i < raw_pixels_size; i++)
		{
			raw_pixels[i] = 0;
		}

		for (auto gui_panel_tab_group : this->gui_panel_tab_groups)
		{
			if (this->gui_panel_tab_groups.size() > 1)
			{
				std::shared_ptr<Image> gui_panel_tab_group_image = gui_panel_tab_group.second->image;
				std::shared_ptr<GUIText> tab_name_text(new GUIText());

				//tab_name_text->font = font;
				//tab_name_text->size = it6->second[1].as<int>();
				//tab_name_text->wrap_length = it6->second.as<int>();
				//tab_name_text->colour = std::shared_ptr<Colour>(new Colour(
				//	(uint8_t)it6->second[0].as<int>(),
				//	(uint8_t)it6->second[1].as<int>(),
				//	(uint8_t)it6->second[2].as<int>(),
				//	(uint8_t)it6->second[3].as<int>())
				//);
				//tab_name_text->text = it6->second.as<std::string>();
				
				/* Draw the tab group to the GUI. */
				Image::normal_blend(
					raw_pixels,
					glm::vec2(0.0),
					maximum_guitab_group_side_positions,
					gui_panel_tab_group_image->get_data(),
					gui_panel_tab_group.second->position,
					gui_panel_tab_group.second->dimensions
				);
			}

			std::shared_ptr<GUIPanel> active_guipanel = gui_panel_tab_group.second->gui_panels[gui_panel_tab_group.second->active_gui_panel_name];

			/* Draw the active GUI panel to the tab group on the GUI. */
			Image::normal_blend(
				raw_pixels,
				glm::vec2(0.0),
				maximum_guitab_group_side_positions,
				active_guipanel->image->get_data(),
				active_guipanel->position + gui_panel_tab_group.second->position,
				active_guipanel->dimensions
			);

			for (auto widget : active_guipanel->widgets)
			{
				/* Draw the widget to the active GUI panel to the tab group on the GUI. */
				Image::normal_blend(
					raw_pixels,
					glm::vec2(0.0),
					maximum_guitab_group_side_positions,
					(uint8_t*)widget.second->image->get_data(),
					widget.second->position + active_guipanel->position + gui_panel_tab_group.second->position,
					widget.second->dimensions
				);
			}
		}
	}

	this->set_dimensions(this->get_image()->get_width(), this->get_image()->get_height(), 0);
}

void Omnia::GUI::set_as_text(std::string text)
{

	if (this->gui_panel_tab_groups.size() > 0)
	{
		if (this->gui_panel_tab_groups.size() == 1)
		{
			for (auto gui_panel_tab_group : this->gui_panel_tab_groups)
			{
				if (gui_panel_tab_group.second->gui_panels.size() == 1)
				{
					for (auto gui_panel : gui_panel_tab_group.second->gui_panels)
					{
						if (gui_panel.second->widgets.size() == 1)
						{
							for (auto widget : gui_panel.second->widgets)
							{
								if (widget.second->gui_element_type == GUIText::TYPE_STRING)
								{
									std::shared_ptr<GUIWidget> gui_widget = widget.second;
									std::shared_ptr<GUIText> gui_text = std::dynamic_pointer_cast<GUIText>(gui_widget);
									gui_text->set_text(text);
								}
							}
						}
					}
				}
			}
		}
	}
	else
	{
		//std::shared_ptr<GUIPanel> gui_panel(new GUIPanel());
		//std::shared_ptr<GUIPanelTabGroup> gui_panel_tab_group(new GUIPanelTabGroup());
		//std::shared_ptr<GUIText> gui_text(new GUIText());

		////gui_text->size;
		////gui_text->font;
		////gui_text->colour;
		////gui_text->set_text(text);

		//gui_panel->name = "Text";
		//gui_panel->widgets.emplace(gui_text->id, gui_text);
		//this->gui_panel_tab_groups.emplace(gui_panel->name, gui_panel_tab_group);
	}

	this->update_image();
}

std::shared_ptr<Omnia::GUIWidget> Omnia::GUI::get_widget(std::string widget_name)
{
	std::shared_ptr<GUIWidget> gui_widget;

	if (this->gui_widget_registry.count(widget_name))
		gui_widget = this->gui_widget_registry.at(widget_name);

	return gui_widget;
}